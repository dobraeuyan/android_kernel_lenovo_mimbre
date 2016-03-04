/*
 * BSD LICENSE
 *
 * Copyright (c) 2013, Intel Corporation
 * All rights reserved.
 *
 * Author: Vinod Koul <vinod.koul@intel.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * Neither the name of Intel Corporation nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 *
 * LGPL LICENSE
 *
 * Copyright (c) 2013, Intel Corporation.
 *
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to
 * the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <poll.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <limits.h>
#include <stdint.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#define __force
#define __bitwise
#define __user
#include "sound/effect_offload.h"
#include "libeffect/libeffect.h"

#define EFFECT_ERR_MAX 128
#define MAX_EFFECTS 16

#ifdef TEST_MODE

#define TEST_FD 22

int my_open(const char *pathname, int flags)
{
	fprintf(stderr, "in %s, for %s, flag %x\n", __func__, pathname, flags);
	return TEST_FD;
}

int my_close(int d)
{
	fprintf(stderr, "in %s for fd %d\n", __func__, d);
	if (d != TEST_FD)
		fprintf(stderr, "returned fd is BAD!!!");
	return 0;
}

#define NUM_DESC 6
#define SIZE_DESC (NUM_DESC * sizeof(effect_descriptor_t))
int my_iotcl(int d, int request, void *arg)
{
	if (d != TEST_FD)
		fprintf(stderr, "returned fd is BAD!!!");

	fprintf(stderr, "in %s for request %x\n", __func__, request);
	switch (request) {
		case SNDRV_CTL_IOCTL_EFFECT_VERSION: {
			fprintf(stderr, "in SNDRV_CTL_IOCTL_EFFECT_VERSION \n");
			int *version = (int *)arg;
			*version = 1;
			return 0;
		}
		case SNDRV_CTL_IOCTL_EFFECT_QUERY_NUM: {
			fprintf(stderr, "in SNDRV_CTL_IOCTL_EFFECT_QUERY_NUM\n");
			int *num = (int *)arg;
			*num = NUM_DESC;
			return 0;
		}
		case SNDRV_CTL_IOCTL_EFFECT_QUERY_CAPS: {
			int i;
			effect_descriptor_t *desc;

			fprintf(stderr, "in SNDRV_CTL_IOCTL_EFFECT_QUERY_CAPS \n");
			struct snd_effect_caps *caps = (struct snd_effect_caps *)arg;
			fprintf(stderr, "buffer size is %x\n", caps->size);
			if (caps->size != SIZE_DESC) {
				fprintf(stderr, "size iz wrong!!! expected %lx is %x", SIZE_DESC, caps->size);
				return -1;
			}
			caps->size = SIZE_DESC;
			desc = (effect_descriptor_t *)caps->buffer;

			for (i = 0; i < NUM_DESC; i++) {
				desc->type.timeLow = 21;
				desc->type.timeMid= 22;
				desc->type.timeHiAndVersion= 23;
				desc->type.clockSeq = 24;
				desc->type.node[0]= 25;
				desc->uuid.timeLow = 31;
				desc->uuid.timeMid= 32;
				desc->uuid.timeHiAndVersion= 33;
				desc->uuid.clockSeq = 34;
				desc->uuid.node[0]= 35;
				desc->apiVersion = 4;
				desc->flags = 5;
				desc->cpuLoad = 6;
				desc->memoryUsage = 7;
				strncpy(desc->name, "one", 8);
				strncpy(desc->implementor, "one_1", 8);
				desc++;
			}
			return 0;
		}
	}
	fprintf(stderr, "no case hit");
	return -1;
}

#endif
