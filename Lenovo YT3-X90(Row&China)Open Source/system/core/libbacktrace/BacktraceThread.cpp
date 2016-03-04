/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <ucontext.h>
#include <unistd.h>

#include <cutils/atomic.h>

#include "BacktraceLog.h"
#include "BacktraceThread.h"
#include "thread_utils.h"

//-------------------------------------------------------------------------
// ThreadEntry implementation.
//-------------------------------------------------------------------------
ThreadEntry* ThreadEntry::list_ = NULL;
pthread_mutex_t ThreadEntry::list_mutex_ = PTHREAD_MUTEX_INITIALIZER;

// Assumes that ThreadEntry::list_mutex_ has already been locked before
// creating a ThreadEntry object.
ThreadEntry::ThreadEntry(pid_t pid, pid_t tid)
    : pid_(pid), tid_(tid), ref_count_(1), mutex_(PTHREAD_MUTEX_INITIALIZER),
      wait_mutex_(PTHREAD_MUTEX_INITIALIZER), wait_value_(0),
      next_(ThreadEntry::list_), prev_(NULL) {
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);
  pthread_condattr_setclock(&attr, CLOCK_MONOTONIC);
  pthread_cond_init(&wait_cond_, &attr);

  // Add ourselves to the list.
  if (ThreadEntry::list_) {
    ThreadEntry::list_->prev_ = this;
  }
  ThreadEntry::list_ = this;
}

ThreadEntry* ThreadEntry::Get(pid_t pid, pid_t tid, bool create) {
  pthread_mutex_lock(&ThreadEntry::list_mutex_);
  ThreadEntry* entry = list_;
  while (entry != NULL) {
    if (entry->Match(pid, tid)) {
      break;
    }
    entry = entry->next_;
  }

  if (!entry) {
    if (create) {
      entry = new ThreadEntry(pid, tid);
    }
  } else {
    entry->ref_count_++;
  }
  pthread_mutex_unlock(&ThreadEntry::list_mutex_);

  return entry;
}

void ThreadEntry::Remove(ThreadEntry* entry) {
  entry->Unlock();
 
  pthread_mutex_lock(&ThreadEntry::list_mutex_);
  if (--entry->ref_count_ == 0) {
    delete entry;
  }
  pthread_mutex_unlock(&ThreadEntry::list_mutex_);
}

// Assumes that ThreadEntry::list_mutex_ has already been locked before
// deleting a ThreadEntry object.
ThreadEntry::~ThreadEntry() {
  if (list_ == this) {
    list_ = next_;
  } else {
    if (next_) {
      next_->prev_ = prev_;
    }
    prev_->next_ = next_;
  }

  next_ = NULL;
  prev_ = NULL;

  pthread_cond_destroy(&wait_cond_);
}

bool ThreadEntry::Wait(int value) {
  timespec ts;
  if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
    BACK_LOGW("clock_gettime failed: %s", strerror(errno));
    abort();
  }
  ts.tv_sec += 5;

  bool wait_completed = true;
  pthread_mutex_lock(&wait_mutex_);
  while (wait_value_ != value) {
    int ret = pthread_cond_timedwait(&wait_cond_, &wait_mutex_, &ts);
    if (ret != 0) {
      BACK_LOGW("pthread_cond_timedwait for value %d failed: %s", value, strerror(ret));
      wait_completed = false; 
      break;
    }
  }
  pthread_mutex_unlock(&wait_mutex_);
  
  return wait_completed;
}


void ThreadEntry::Wake() {
  pthread_mutex_lock(&wait_mutex_);
  wait_value_++;
  pthread_mutex_unlock(&wait_mutex_);

  pthread_cond_signal(&wait_cond_);
}

void ThreadEntry::CopyUcontextFromSigcontext(void* sigcontext) {
  ucontext_t* ucontext = reinterpret_cast<ucontext_t*>(sigcontext);
  // The only thing the unwinder cares about is the mcontext data.
  memcpy(&ucontext_.uc_mcontext, &ucontext->uc_mcontext, sizeof(ucontext->uc_mcontext));
}

//-------------------------------------------------------------------------
// BacktraceThread functions.
//-------------------------------------------------------------------------
static pthread_mutex_t g_sigaction_mutex = PTHREAD_MUTEX_INITIALIZER;

static void SignalHandler(int, siginfo_t*, void* sigcontext) {
  ThreadEntry* entry = ThreadEntry::Get(getpid(), gettid(), false);
  if (!entry) {
    BACK_LOGE("pid %d, tid %d entry not found", getpid(), gettid());
    return;
  }

  entry->CopyUcontextFromSigcontext(sigcontext);

  // Indicate the ucontext is now valid.
  entry->Wake();

  // Pause the thread until the unwind is complete. This avoids having
  // the thread run ahead causing problems.
  if (entry->Wait(2)) {
    // Do not remove the entry here because that can result in a deadlock
    // if the code cannot properly send a signal to the thread under test.
    entry->Wake();
  } else {
    // At this point, it is possible that entry has been freed, so just exit.
    BACK_LOGE("Timed out waiting for unwind thread to indicate it completed.");
  }
}

BacktraceThread::BacktraceThread(BacktraceImpl* impl, pid_t tid, BacktraceMap* map)
    : BacktraceCurrent(impl, map) {
  tid_ = tid;
}

BacktraceThread::~BacktraceThread() {
}

bool BacktraceThread::Unwind(size_t num_ignore_frames, ucontext_t* ucontext) {
  if (ucontext) {
    // Unwind using an already existing ucontext.
    return impl_->Unwind(num_ignore_frames, ucontext);
  }

  // Prevent multiple threads trying to set the trigger action on different
  // threads at the same time.
  if (pthread_mutex_lock(&g_sigaction_mutex) < 0) {
    BACK_LOGW("sigaction failed: %s", strerror(errno));
    return false;
  }

  ThreadEntry* entry = ThreadEntry::Get(Pid(), Tid());
  entry->Lock();

  struct sigaction act, oldact;
  memset(&act, 0, sizeof(act));
  act.sa_sigaction = SignalHandler;
  act.sa_flags = SA_RESTART | SA_SIGINFO | SA_ONSTACK;
  sigemptyset(&act.sa_mask);
  if (sigaction(THREAD_SIGNAL, &act, &oldact) != 0) {
    BACK_LOGE("sigaction failed: %s", strerror(errno));
    ThreadEntry::Remove(entry);
    pthread_mutex_unlock(&g_sigaction_mutex);
    return false;
  }

  if (tgkill(Pid(), Tid(), THREAD_SIGNAL) != 0) {
    BACK_LOGE("tgkill %d failed: %s", Tid(), strerror(errno));
    sigaction(THREAD_SIGNAL, &oldact, NULL);
    ThreadEntry::Remove(entry);
    pthread_mutex_unlock(&g_sigaction_mutex);
    return false;
  }

  // Wait for the thread to get the ucontext.
  bool wait_completed = entry->Wait(1);

  // After the thread has received the signal, allow other unwinders to
  // continue.
  sigaction(THREAD_SIGNAL, &oldact, NULL);
  pthread_mutex_unlock(&g_sigaction_mutex);

  bool unwind_done = false;
  if(wait_completed) {
    unwind_done = impl_->Unwind(num_ignore_frames, entry->GetUcontext());

    // Tell the signal handler to exit and release the entry.
    entry->Wake();
    // Wait for the thread to indicate it is done with the ThreadEntry.
    if (!entry->Wait(3)) {
      // Send a warning, but do not mark as a failure to unwind.
      BACK_LOGW("Timed out waiting for signal handler to indicate it finished.");
    }
  } else {
    BACK_LOGE("Timed out waiting for signal handler to get ucontext data.");
  }

  ThreadEntry::Remove(entry);

  return unwind_done;
}
