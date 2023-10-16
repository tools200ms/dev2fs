/* stringBuffer.c --- Functions for character strings operations.
 * Copyright (C) 2012 Mateusz Piwek
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

	if( buf == NULL ) return NULL;

	int base_len = strlen( base_path );
	MSG_DEBUG_DEC( "base_len", base_len );
	buf->relative_path_max_length = REL_PATH_BUFFSIZE;
	buf->relative_path_length = 0;

	if( 	(buf->path_buff = 
					malloc( sizeof (char) * ( 	base_len + 
														buf->relative_path_max_length + 1 ) ) ) 
			== NULL )
		return NULL;

	strcpy( buf->path_buff, base_path );
	
	buf->relative_path = buf->path_buff + base_len;
	
	return buf;
}

void strbuff_destroy( StringBuffer *buf )
{
	//if( pb->path_buff != NULL )
	free( buf->path_buff );
	free( buf );
}

// static int _strbuff_resize( StringBuffer *buf, int new_relative_path_max_length );
static int setMaxLengthOfBuffPath( StringBuffer *buf, int relative_path_max_length )
{
	MSG_DEBUG( "relative_path_max_length", "%d", relative_path_max_length );
	
	int base_len = buf->relative_path - buf->path_buff;
	
	buf->path_buff = realloc( 	buf->path_buff, 
										sizeof (char) * ( base_len + 
																relative_path_max_length + 1 ) );
	
	if( buf->path_buff == NULL ) return -1;

	buf->relative_path = buf->path_buff + base_len;
	buf->relative_path_max_length = relative_path_max_length;

	MSG_DEBUG_DEC( "base_len + relative_path_max_length", base_len + relative_path_max_length );

	return 0;
}

//char *strbuff_apply( StringBuffer *buf, char *str )
char *strbuff_setFullPath( StringBuffer *buf, const char *relative_path )
{
	MSG_DEBUG("pb->relative_path_max_length", "%d", buf->relative_path_max_length);
	buf->relative_path_length = strlen( relative_path );

	if( (buf->relative_path_length) > (buf->relative_path_max_length) )
	{
		setMaxLengthOfBuffPath( buf, buf->relative_path_length );
		//relative_path_max_length = strlen( relative_path );
	}
	
	strcpy( buf->relative_path, relative_path );
	
	return buf->path_buff;
}

//char *strbuff_add( StringBuffer *buf, char *str )
char *strbuff_addPostfix( StringBuffer *buf, const char *str )
{
	int str_len = strlen( str );
	
	if( ((buf->relative_path_length) + str_len) > (buf->relative_path_max_length) )
	{
		setMaxLengthOfBuffPath( buf, (buf->relative_path_length) + str_len );
		//relative_path_max_length = strlen( relative_path );
	}
	
	(buf->relative_path_length) += str_len;
	
	strcat( buf->relative_path, str );
	
	return buf->path_buff;
}

StrBuff_base_idx strbuff_getBaseIdx( StringBuffer *buf )
{
	return buf->relative_path - buf->path_buff;
}


// StrBuff_base_idx strbuff_setToBase( StringBuffer *buf )
StrBuff_base_idx strbuff_resetRelativePath( StringBuffer *buf )
{
	int rel_len = strlen( buf->relative_path );

	(buf->relative_path_length) = 0;
	(buf->relative_path) += rel_len;
	(buf->relative_path_max_length) -= rel_len;

	return buf->relative_path - buf->path_buff;
}

// void strbuff_alterBase( StringBuffer *buf, StrBuff_base_idx relative_idx )
void strbuff_setRelativePath( StringBuffer *buf, StrBuff_base_idx relative_idx )
{
	char *new_relative_path = buf->path_buff + relative_idx;
	int rel_path_diff = (buf->relative_path) - new_relative_path;

//	MSG_DEBUG("pb->relative_path", "%d", pb->relative_path );
//	MSG_DEBUG("new_relative_path", "%d", new_relative_path );
//	MSG_DEBUG("relative_idx", "%d", relative_idx );

	(buf->relative_path) = new_relative_path;

	(buf->relative_path_length) -= rel_path_diff;
	(buf->relative_path_max_length) += rel_path_diff;
}

// inline char *strbuff_getStrStartingFromIdx( struct path_buffer *pb, StrBuff_base_idx start_from )
char *strbuff_getPathWithBase( struct path_buffer *pb, StrBuff_base_idx start_from )
{
	return pb->path_buff + start_from;
}

char *strbuff_getRelativeStr( StringBuffer *buf )
{
	return buf->relative_path;
}

// void strbuff_setNewBase( StringBuffer *buf, char *new_base_path )
void setNewBase( StringBuffer *buf, char *new_base_path )
{
	int 	new_base_path_len = strlen( new_base_path ), 
			base_len = (buf->relative_path - buf->path_buff), 
			base_path_len_diff = new_base_path_len - base_len;
	
	if( buf->relative_path_max_length < base_path_len_diff )
		setMaxLengthOfBuffPath( buf, base_path_len_diff );
	
	strcpy( buf->path_buff, new_base_path );
	buf->relative_path = (buf->path_buff) + new_base_path_len;
	buf->relative_path_max_length -= base_path_len_diff;
}

void strbuff_printBase( StringBuffer *buf, FILE *stream )
{
	char tmp_c = *(buf->relative_path);
	*(buf->relative_path) = '\0';

	fprintf( stream, "%s\n", buf->path_buff );
	//fputc( '\0', stream );
	
	*(buf->relative_path) = tmp_c;
}

