/* stats.c --- implementaion of statistics of fs operations.
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
#include <stdarg.h>

#include "op_stats.h"
#include "../lib/messages.h"


Stats *stat_init() {

	Stats *s = malloc( sizeof (Stats) );

	memset( s, 0, sizeof (Stats) );
	s->t_s = malloc( sizeof(struct timespec) );
	s->t_e = malloc( sizeof(struct timespec) );
	s->run = NORUN;

	return s;
}

void stat_op_start(Stats* s, char* op_name, int argc, ...)
{
	va_list op_data;
	char *path, *path2 = NULL;
	char *format = "%s: %s";

	if( s->run == RUN ) {
		//Raise panic
		va_start(op_data, argc);

		path = va_arg(op_data, char *);
		fprintf( stderr, "No-atomic call of: %s: %s\n", op_name, path);
		exit(11);
	}

	s->run = RUN;

	va_start(op_data, argc);

	path = va_arg(op_data, char *);
	if(argc == 2) {
		path2 = va_arg(op_data, char *);
		format = "%s: %s:%s";
	}

	va_end(op_data);

	fprintf( stdout, format, op_name, path, path2 );

	clock_gettime(CLOCK_MONOTONIC, s->t_s);
}


void stat_op_end(Stats *s) {

	if( s->run == NORUN ) {
		//Raise panic

		exit(12);
	}

	clock_gettime(CLOCK_MONOTONIC, s->t_e);

	time_t sec_el = (s->t_e)->tv_sec - (s->t_s)->tv_sec;
	// elapsed nsec
	long int nsec_el = (s->t_e)->tv_nsec - (s->t_s)->tv_nsec;
	if(nsec_el < 0) {
		--sec_el;
		nsec_el += 1000000000;
	}

	fprintf(stdout, "   (%ld,%09ld sec)\n", sec_el, nsec_el);

	s->run = NORUN;
}

void stat_destroy(Stats *s) {

	free(s);
}
