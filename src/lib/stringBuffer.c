/* stringBuffer.c --- Functions for character strings operations.
 * Copyright (C) 2012, 2023 Mateusz Piwek
 * 
 * This file is part of TeaFS.
 * 
 * TeaFS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * TeaFS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TeaFS.  If not, see <http://www.gnu.org/licenses/>.
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
	int base_len = strlen( base_path );

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
	//if( pb->path_buff != NULL )
	free( buf->p_buf );
	free( buf );
}


char *strbuff_setFullPath( StringBuffer *buf, const char *rel_path )
{
	int rp_len = strlen( rel_path );
	MSG_DEBUG( 			"     (old) base path", "%s", buf->p_buf );
	MSG_DEBUG( 			"            rel len.", "%d", buf->rp_buf_len );

	if( buf->rp_buf_len < rp_len ) {
		char *p_buf;
		int base_len = buf->rp_buf_start - buf->p_buf,
			new_rp_buf_len = rp_len + REL_PATH_BUFF_MARGIN;

		(buf->rp_buf_start)[0] = '\0';
		p_buf = malloc( (sizeof (char)) * (new_rp_buf_len + 1) );
		if(p_buf == NULL)
			return NULL;

		strcpy(p_buf, buf->p_buf);
		free(buf->p_buf);
		buf->p_buf = p_buf;
		buf->rp_buf_start = buf->p_buf + base_len;
		buf->rp_buf_len = new_rp_buf_len;

		MSG_DEBUG( 			"   (changed) rel len.", "%d", buf->rp_buf_len );
	}

	strcpy(buf->rp_buf_start, rel_path);

	MSG_DEBUG( 			"     (new) base path", "%s", buf->p_buf );

	return buf->p_buf;
}

//char *strbuff_add( StringBuffer *buf, char *str )
char *strbuff_addPostfix( StringBuffer *buf, const char *str )
{

	return NULL;
}

StrBuff_base_idx strbuff_getBaseIdx( StringBuffer *buf )
{
	return 9;
}


// StrBuff_base_idx strbuff_setToBase( StringBuffer *buf )
StrBuff_base_idx strbuff_resetRelativePath( StringBuffer *buf )
{

	return 0;
}

// void strbuff_alterBase( StringBuffer *buf, StrBuff_base_idx relative_idx )
void strbuff_setRelativePath( StringBuffer *buf, StrBuff_base_idx relative_idx )
{

}

// inline char *strbuff_getStrStartingFromIdx( struct path_buffer *pb, StrBuff_base_idx start_from )
char *strbuff_getPathWithBase( struct path_buffer *pb, StrBuff_base_idx start_from )
{
	return 0;
}

char *strbuff_getRelativeStr( StringBuffer *buf )
{
	return 0;
}

// void strbuff_setNewBase( StringBuffer *buf, char *new_base_path )
void setNewBase( StringBuffer *buf, char *new_base_path )
{

}

void strbuff_printBase( StringBuffer *buf, FILE *stream )
{

}

