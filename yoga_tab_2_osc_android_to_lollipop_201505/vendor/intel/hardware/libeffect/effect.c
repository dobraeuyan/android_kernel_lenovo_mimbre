/*
 * BSD LICENSE
 *
 * effect library for audio effect offload in alsa
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
 * effect library for audio effect offload in alsa
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
#include "test/test.h"
#define ioctl my_iotcl
#define open my_open
#define close my_close
#endif
struct libeffect {
	unsigned int card;
	bool init;

	int fd;
	char error[EFFECT_ERR_MAX];

	int num_effects;
	effect_descriptor_t *descriptors;
	size_t desc_size;
	size_t desc_read_size;
};

struct effect {
	struct libeffect *lib;
	effect_uuid_t uuid;
	__u32 device;
	__u32 pos;
	__u32 mode;
};

static int oops(struct libeffect *lib, int e, const char *fmt, ...)
{
	va_list ap;
	int sz;

	va_start(ap, fmt);
	vsnprintf(lib->error, EFFECT_ERR_MAX, fmt, ap);
	va_end(ap);
	sz = strlen(lib->error);

	if (errno)
		snprintf(lib->error + sz, EFFECT_ERR_MAX - sz,
			": %s", strerror(e));
	return e;
}

const char *libeffect_get_error(struct libeffect *lib)
{
	return lib->error;
}

static struct libeffect bad_lib = {
	.fd = -1,
};

bool is_libeffect_ready(struct libeffect *lib)
{
	return ((lib->fd > 0) && lib->init) ? true : false;
}

static int get_effect_kernel_version(struct libeffect *lib)
{
	int version = 0;

	if (ioctl(lib->fd, SNDRV_CTL_IOCTL_EFFECT_VERSION, &version)) {
		oops(lib, errno, "cant read kernel version");
		return -1;
	}
	return version;
}

static int get_effect_descriptors(struct libeffect *lib)
{
	struct snd_effect_caps caps;

	caps.size = lib->desc_size;
	caps.buffer_ptr = (uintptr_t)lib->descriptors;

	if (ioctl(lib->fd, SNDRV_CTL_IOCTL_EFFECT_QUERY_CAPS, &caps)) {
		oops(lib, errno, "effects ioctl query failed");
		return errno;
	}
	lib->desc_read_size = caps.size;
	return 0;
}

struct libeffect *libeffect_init(struct effect_config *config)
{
	struct libeffect *lib;
	char fn[256];
	int version;

	if (!config) {
		oops(&bad_lib,  -EINVAL, "passed bad config");
		return &bad_lib;
	}

	lib = malloc(sizeof(*lib));
	if (!lib) {
		oops(&bad_lib, errno, "cannot allocate effectlib object");
		return &bad_lib;
	}

	lib->card = config->card_num;

	/* lets check the sound card device */
	snprintf(fn, sizeof(fn), "/dev/snd/controlC%u", lib->card);

	lib->fd = open(fn, O_RDWR);
	if (lib->fd < 0) {
		oops(&bad_lib, errno, "cannot open device '%s'", fn);
		goto open_fail;
	}

	/* check if version supported */
	version = get_effect_kernel_version(lib);
	if (version <= 0) {
		oops(&bad_lib, -ENXIO, "invalid kernel version '%s'", fn);
		goto open_fail;
	}

	/* query the capabilities */
	if (ioctl(lib->fd, SNDRV_CTL_IOCTL_EFFECT_QUERY_NUM, &lib->num_effects)) {
		oops(&bad_lib, errno, "failed to query effects supported '%s'", fn);
		goto open_fail;
	}

	if ((lib->num_effects < 0) || (lib->num_effects > MAX_EFFECTS)) {
		oops(&bad_lib, errno, "invalid effects %d returned by kernel '%s'", lib->num_effects, fn);
		goto open_fail;
	}

	/* allocate memory for desc */
	lib->desc_size = sizeof(effect_descriptor_t) * lib->num_effects;
	lib->descriptors = malloc(lib->desc_size);
	if (!lib->descriptors) {
		oops(&bad_lib, errno, "failed to allocate mem for descriptors '%s'", fn);
		goto open_fail;
	}

	if (get_effect_descriptors(lib)) {
		oops(&bad_lib, errno, "effects query failed '%s'", fn);
		goto desc_fail;
	}

	lib->init = true;
	return lib;

desc_fail:
	free(lib->descriptors);

open_fail:
	close(lib->fd);
	free(lib);
	return &bad_lib;
}

void libeffect_destroy(struct libeffect *lib)
{
	close(lib->fd);
	free(lib->descriptors);
	free(lib);
}

int effect_num_query(struct libeffect *lib)
{
	if (is_libeffect_ready(lib) == false)
		return oops(lib, -ENODEV, "device/lib not ready");

	return lib->num_effects;
}

int effect_query_caps(struct libeffect *lib, size_t size, effect_descriptor_t *descriptors)
{
	if (!lib)
		return -1;

	if (is_libeffect_ready(lib) == false)
		return oops(lib, -ENODEV, "device/lib not ready");

	if (size < lib->desc_read_size)
		return oops(lib, -ENOMEM, "insufficent memory passed");

	memcpy(descriptors, lib->descriptors, lib->desc_read_size);

	return lib->desc_read_size;
}

int effect_query_descriptor_by_index(struct libeffect *lib, int index, effect_descriptor_t *descriptor)
{
	if (!lib)
		return -1;

	if (!descriptor)
		return oops(lib, -ENODEV, "invalid descriptor");

	if (is_libeffect_ready(lib) == false)
		return oops(lib, -ENODEV, "device/lib not ready");

	if (index >= lib->num_effects)
		return oops(lib, -EINVAL, "invalid index value %d", index);

	memcpy(descriptor, lib->descriptors + index, sizeof(*descriptor));
	return 0;
}

int effect_query_descriptor_by_uuid(struct libeffect *lib, effect_uuid_t uuid,
		effect_descriptor_t *descriptor)
{
	effect_descriptor_t *_d;
	int i;

	if (!lib)
		return -1;

	if (!descriptor)
		return oops(lib, -ENODEV, "invalid descriptor");

	if (is_libeffect_ready(lib) == false)
		return oops(lib, -ENODEV, "device/lib not ready");

	_d = lib->descriptors;
	for (i = 0; i < lib->num_effects; i++) {
		if (!strncmp((char *)&_d->uuid, (char *)&uuid, sizeof(effect_uuid_t))) {
			memcpy(descriptor, _d, sizeof(*_d));
			return 0;
		}
		_d++;
	}
	return oops(lib, -EIO, "uuid not found");
}

struct effect *effect_create(struct libeffect *lib, effect_uuid_t uuid, __u32 pos,
		int stream, __u32 device)
{
	struct snd_effect snd_effect;
	struct effect *effect;

	if (!lib)
		return NULL;

	if (is_libeffect_ready(lib) == false)
		return NULL;

	effect = malloc(sizeof(*effect));
	if (!effect) {
		oops(lib, errno, "failed to alloc memory for effect");
		goto free_mem;
	}

	memcpy(&snd_effect.uuid, &uuid, 16);
	if (stream == 0xFF)
		snd_effect.device = device;
	else
		snd_effect.device = stream;
	snd_effect.pos = pos;
	snd_effect.mode = device; /* mode = 0 for local effect */
				  /* mode = 0xFF for Global Effect */

	if (ioctl(lib->fd, SNDRV_CTL_IOCTL_EFFECT_CREATE, &snd_effect)) {
		oops(lib, errno, "kernel failed to create effect");
		goto free_mem;
	}

	/* effect created in kernel, now create desc for it */
	memcpy(&effect->uuid, &uuid, 16);
	effect->lib = lib;
	effect->device = snd_effect.device;
	effect->pos = snd_effect.pos;
	effect->mode = snd_effect.mode;
	/* todo: add this ptr in lib... for cleanup
	 */
	return effect;

free_mem:
	free(effect);
	return NULL;
}

int effect_destroy(struct effect *handle)
{
	struct libeffect *lib;
	struct snd_effect snd_effect;

	if (!handle)
		return oops(&bad_lib, -EINVAL, "invalid argument");

	lib = handle->lib;

	if (is_libeffect_ready(lib) == false)
		return oops(lib, -ENODEV, "device/lib not ready");

	memcpy(&snd_effect.uuid, &handle->uuid, 16);
	snd_effect.device = handle->device;
	snd_effect.pos = handle->pos;
	snd_effect.mode = handle->mode;

	if (ioctl(lib->fd, SNDRV_CTL_IOCTL_EFFECT_DESTROY, &snd_effect))
		return oops(lib, errno, "kernel failed to destroy effect");

	free(handle);
	return 0;
}

int effect_set_params(struct effect *handle, size_t size, const char *buffer)
{
	struct libeffect *lib;
	struct snd_effect_params params;

	if (!handle)
		return oops(&bad_lib, -EINVAL, "invalid argument");

	lib = handle->lib;

	if (is_libeffect_ready(lib) == false)
		return oops(lib, -ENODEV, "device/lib not ready");

	memcpy(&params.uuid, &handle->uuid, 16);
	params.device = handle->device;
	params.size = size;
	params.buffer_ptr = (uintptr_t)buffer;

	if (ioctl(lib->fd, SNDRV_CTL_IOCTL_EFFECT_SET_PARAMS, &params))
		return oops(lib, errno, "kernel failed to set effect params");

	return 0;
}

int effect_get_params(struct effect *handle, size_t size, char *buffer)
{
	struct libeffect *lib;
	struct snd_effect_params params;

	if (!handle)
		return oops(&bad_lib, -EINVAL, "invalid argument");

	lib = handle->lib;

	if (is_libeffect_ready(lib) == false)
		return oops(lib, -ENODEV, "device/lib not ready");

	memcpy(&params.uuid, &handle->uuid, 16);
	params.device = handle->device;
	params.size = size;
	params.buffer_ptr = (uintptr_t)buffer;

	if (ioctl(lib->fd, SNDRV_CTL_IOCTL_EFFECT_GET_PARAMS, &params))
		return oops(lib, errno, "kernel failed to get effect params");

	return 0;
}
