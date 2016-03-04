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



#ifndef LIB_EFFECT_H
#define LIB_EFFECT_H

#include <hardware/audio_effect.h>

struct effect_config {
	int card_num;
	int log_level;
};

struct libeffect;
struct effect;

/* libeffect_init- initialize the effect library instance
 *
 * @effect_config: config for effect initialization
 *
 * return: bad_effect on error. valid instance on sucess
 * use is_effect_ready() for further check
 */
struct libeffect *libeffect_init(struct effect_config *config);

/* libeffect_destroy- destroy the effect library instance
 *
 * @handle: Previously intialized effect instance
 *
 */
void libeffect_destroy(struct libeffect *lib);

/* effect_create - create the effect instance
 *
 * @handle: Previously intialized effect instance
 * @uuid: effect uuid
 * @position: effect insert position
 * @stream: streaming interface for effect insertion
 * 	pcmCxDyp -> y. comprCpDq -> q
 * 	0xFF represent global effect
 * @device: Backend for global device
 *	Since global effects can be handled differently by DSP, this value helps
 *	DSP to identify on which backend the global effect will be placed.
 *	Valid for global effects only.
 *	Value: Headset/Speaker/etc
 *
 * return: valid effect instance on creation. Null on error
 */
struct effect *effect_create(struct libeffect *lib, effect_uuid_t uuid, __u32 pos,
		int stream, __u32 device);

/* effect_destroy: destroy the Previously intialized effect instance
 *
 * @handle: Previously intialized effect instance
 */
int effect_destroy(struct effect *handle);

/* effect_set_params: set a binary blob of params for an effect
 *
 * @handle: Previously intialized effect instance
 * @size: size of the buffer passed
 * @buffer: pointer to the buffer
 */
int effect_set_params(struct effect *handle, size_t size, const char *buffer);

/* effect_get_params: get a binary blob of params for an effect
 *
 * @handle: Previously intialized effect instance
 * @size: size of the buffer passed
 * @buffer: pointer to the buffer: this may contain data which can be used for
 * query
 *
 * return: size read on success
 * -1 on error
 */
int effect_get_params(struct effect *handle, size_t size, char *buffer);


/* effect_num_query: query the number of effects supported by DSP
 *
 * @handle: Previously intialized effect lib instance
 *
 * returns: number of effects supported
 *	< 0 for error
 */
int effect_num_query(struct libeffect *lib);

/* effect_query_caps:  query the caps of effects supported by DSP
 *
 * @handle: Previously intialized effect instance
 * @size: size of buffer for effect descriptors.  This should be
 *	effect_descriptor_t times number of effects supported as queried in
 *	effect_num_query
 * @descriptor: pointer to array of n descriptors
 */
int effect_query_caps(struct libeffect *lib, size_t size, effect_descriptor_t
		*descriptor);

int effect_query_descriptor_by_index(struct libeffect *lib, int index, effect_descriptor_t *descriptor);
int effect_query_descriptor_by_uuid(struct libeffect *lib, effect_uuid_t uuid,
		effect_descriptor_t *descriptor);
/* is_libeffect_ready: check effect lib instance status
 *
 * @handle: Previously intialized effect instance
 */
bool is_libeffect_ready(struct libeffect *lib);

const char *libeffect_get_error(struct libeffect *lib);

#endif
