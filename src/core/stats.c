/* resources.c --- implementaion of statistics of fs operations.
 * Copyright (C) 2023 Mateusz Piwek
 * 
 * This file is part of Dev2FS.
 * 
 * Dev2FS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Dev2FS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Dev2FS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <string.h>

#include "stats.h"
#include "../lib/messages.h"


Stats *stat_init() {

	Stats *s = malloc( sizeof (Stats) );

	memset( s, 0, sizeof (Stats) );

	return s;
}

inline void op_start(Stats *s) {

	clock_gettime(CLOCK_MONOTONIC, &(s->t_s));
}

inline void op_end(Stats *s) {

	clock_gettime(CLOCK_MONOTONIC, &(s->t_e));
}

void stat_destroy(Stats *s) {

	free(s);
}
