/* stringBuffer.h --- Header file for stringBuffer.c.
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


#ifndef _TEAFS_ORG_LIB_STRING_BUFFER_H_
#define _TEAFS_ORG_LIB_STRING_BUFFER_H_

#define REL_PATH_BUFFSIZE 0

typedef struct path_buffer
{
	char *path_buff, *relative_path;
	short int relative_path_length, relative_path_max_length;
}
StringBuffer;

typedef short int StrBuff_base_idx;

StringBuffer * strbuff_init( char *base_path );

void 				strbuff_destroy( StringBuffer *buf );

//char *			getFullPath( StringBuffer *buf, char *relative_path );

// add (...) multi arguments
char *strbuff_setFullPath( StringBuffer *buf, const char *relative_path );

char *			strbuff_addPostfix( StringBuffer *buf, const char *postfix );


StrBuff_base_idx strbuff_getBaseIdx( StringBuffer *buf );
StrBuff_base_idx 	strbuff_resetRelativePath( StringBuffer *buf );
void 					strbuff_setRelativePath( StringBuffer *buf, StrBuff_base_idx relative_idx );

char *	strbuff_getPathWithBase( StringBuffer *buf, StrBuff_base_idx start_from );
char *strbuff_getRelativeStr( StringBuffer *buf );

void 				setNewBase( StringBuffer *buf, char *new_base_path );

void 	strbuff_printBase( StringBuffer *buf, FILE *stream );

#endif // _TEAFS_ORG_LIB_STRING_BUFFER_H_
