/*
 * BSD LICENSE
 *
 * test app for libffect
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
 * test app for libffect
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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <pthread.h>

#include <libeffect/libeffect.h>

static void help(void)
{
	printf("Usage: effecttest <config options> -i <cmd options> [command] [paramdata file] -k\n");
	printf("\nAvailable options:\n");
	printf("  -h,--help		shows this help message\n");
	printf("  -i,--init		lib instantiation\n");
	printf("  -k,--kill		lib destroy\n");
	printf("  -d,--debug <level>	lib debug level\n");
	printf("	level - 0 - binary dump\n");
	printf("	level - 1 - debug\n");
	printf("	level - 2 - error\n");
	printf("	level 2 is default\n");
	printf("	Note: -d has to come before read/write for proper debug output\n\n");

	exit(EXIT_FAILURE);
}

void check_lib_instance(struct libeffect *lib)
{
	if (!lib) {
		fprintf(stderr, "Lib not init!!!\n");
		exit(EXIT_FAILURE);
	}
	if (is_libeffect_ready(lib) == false) {
		fprintf(stderr, "Lib not ready: %s\n", libeffect_get_error(lib));
		exit(EXIT_FAILURE);
	}
	return;
}

int main(int argc, char **argv)
{
	int ret = 0;
	struct libeffect *lib = NULL;
	int c;
	int card_num  = 1, log = 0, num = 0;

	if(argc < 2)
		help();

	while ((c = getopt(argc, argv, "hikndc:l:")) != -1) {
		switch (c) {
		case 'h':
			fprintf(stderr, "in help()\n");
			help();
			break;
		case 'c':
			card_num =  strtol(optarg, NULL, 0);
			fprintf(stderr, "got card as %d\n", card_num);
			break;
		case 'l':
			log = strtol(optarg, NULL, 0);
			fprintf(stderr, "got log as %d\n", log);
			break;
		case 'i': {
			struct effect_config conf;
			fprintf(stderr, "in init()\n");

			if(lib) {
				fprintf(stderr, "Lib already init!!!\n");
				exit(EXIT_FAILURE);
			}

			fprintf(stderr, "Creating a lib context first (option '-i')\n");
			conf.card_num = card_num;
			lib = libeffect_init(&conf);
			if (!lib) {
				fprintf(stderr, "No lib context\n");
				return -1;
			}
			if (is_libeffect_ready(lib) == false) {
				fprintf(stderr, "Lib not ready. err: %s\n", libeffect_get_error(lib));
				return -1;
			}
			break;
		}
		case 'n':

			check_lib_instance(lib);
			num = effect_num_query(lib);
			if (num <= 0) {
				fprintf(stderr, "effect_num_query failed :%s\n", libeffect_get_error(lib));
				exit(EXIT_FAILURE);
			}
			fprintf(stderr, "effect_num_query returned %d effects\n", num);
			break;

		case 'd': {
			int sz, i;
			effect_descriptor_t *desc, *_d;

			check_lib_instance(lib);
			fprintf(stderr, "querying desciptors\n");

			sz = num * sizeof(*desc);
			desc = malloc(sz);
			if (!desc) {
				fprintf(stderr, "no mem\n");
				exit(EXIT_FAILURE);
			}

			ret = effect_query_caps(lib, sz, desc);
			if (ret <= 0) {
				fprintf(stderr, "effect query failed: %s\n", libeffect_get_error(lib));
				exit(EXIT_FAILURE);
			}

			fprintf(stderr, "dumping desciptors %d bytes. Sz sent %d\n", ret, sz);
			for (i = 0; i < num; i++) {
				_d = desc + i;
				fprintf(stderr, "*************DESC %d/%d *********\n", i + 1, num);
				fprintf(stderr, "type.timeLow %d\n", _d->type.timeLow);
				fprintf(stderr, "type.timeMid %d\n", _d->type.timeMid);
				fprintf(stderr, "type.timeHiAndVersion %d\n", _d->type.timeHiAndVersion);
				fprintf(stderr, "type.clockSeq %d\n", _d->type.clockSeq);
				fprintf(stderr, "type.node %d\n", _d->type.node[0]);
				fprintf(stderr, "uuid.timeLow %d\n", _d->uuid.timeLow);
				fprintf(stderr, "uuid.timeMid %d\n", _d->uuid.timeMid);
				fprintf(stderr, "uuid.timeHiAndVersion %d\n", _d->uuid.timeHiAndVersion);
				fprintf(stderr, "uuid.clockSeq %d\n", _d->uuid.clockSeq);
				fprintf(stderr, "uuid.node %d\n", _d->uuid.node[0]);
				fprintf(stderr, "apiVersion %d\n", _d->apiVersion);
				fprintf(stderr, "flags %d\n", _d->flags);
				fprintf(stderr, "cpuLoad %d\n", _d->cpuLoad);
				fprintf(stderr, "memoryUsage %d\n", _d->memoryUsage);
				fprintf(stderr, "name %s\n", _d->name);
				fprintf(stderr, "implementor %s\n", _d->implementor);
			}
			free(desc);
			break;
		}
		case 'k':
			fprintf(stderr, "Destroy the lib context (option '-k')\n");
			if (!lib) {
				fprintf(stderr, "No lib context\n");
				return -1;
			}
			libeffect_destroy(lib);
			break;
		}
	}
	exit(EXIT_SUCCESS);
}
