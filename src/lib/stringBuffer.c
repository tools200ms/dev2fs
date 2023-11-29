/* stringBuffer.c --- Functions for character strings operations.
 * Copyright (C) 2012, 2023 Mateusz Piwek
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

/** for debug definitions **/
#include <stdio.h>
#include "messages.h"
/** ********************* **/

#include "stringBuffer.h"


StringBuffer *strbuff_init( char *base_path )
{
	StringBuffer *buf = malloc( sizeof (StringBuffer) );

	if( buf == NULL )
		return NULL;

	// Initialise:
	size_t base_len = strlen( base_path );

	char *p_buf =
		malloc( (sizeof (char)) * (base_len + REL_PATH_DEFAULT_BUFFSIZE + 1) );

	if( (buf->p_buf = p_buf) == NULL )
		return NULL;

	buf->rp_buf_len = REL_PATH_DEFAULT_BUFFSIZE;

	strcpy( p_buf, base_path );
	buf->rp_buf_start = p_buf + base_len;
	
	return buf;
}

void strbuff_destroy( StringBuffer *buf )
{
	free( buf->p_buf );
	free( buf );
}


char *strbuff_setFullPath(StringBuffer *buf, const char *rel_path)
{
	size_t rp_len = strlen( rel_path );
	MSG_DEBUG_STR( "     (old) base path", buf->p_buf );
	MSG_DEBUG_   ( "            rel len.", "%lu", buf->rp_buf_len );

	if( buf->rp_buf_len < rp_len ) {
		size_t base_len = buf->rp_buf_start - buf->p_buf,
			new_rp_buf_len = rp_len + REL_PATH_BUFF_MARGIN;

		//(buf->rp_buf_start)[0] = '\0';
		buf->p_buf = realloc( buf->p_buf, base_len + (sizeof (char)) * (new_rp_buf_len + 1) );

		if(buf->p_buf == NULL)
			return NULL;

		//strcpy(p_buf, buf->p_buf);
		//free(buf->p_buf);
		//buf->p_buf = p_buf;
		buf->rp_buf_start = buf->p_buf + base_len;
		buf->rp_buf_len = new_rp_buf_len;

		MSG_DEBUG_( "   (changed) rel len.", "%lu", buf->rp_buf_len );
	}

	strcpy(buf->rp_buf_start, rel_path);

	MSG_DEBUG_STR( "     (new) base path", buf->p_buf );

	return buf->p_buf;
}

void strbuff_release(char *path) {

	free(path);
}

