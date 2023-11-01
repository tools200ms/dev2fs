/* stringBuffer.h --- Header file for stringBuffer.c.
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

#ifndef _DEV2FS_200MS_NET__LIB_STRING_BUFFER_H_
#define _DEV2FS_200MS_NET__LIB_STRING_BUFFER_H_

#define REL_PATH_MAX_BUFFSIZE 4000
#define REL_PATH_DEFAULT_BUFFSIZE 0
#define REL_PATH_BUFF_MARGIN 0

typedef struct path_buffer
{
	char *p_buf, *rp_buf_start;
	short int rp_buf_len;
}
StringBuffer;

typedef short int StrBuff_base_idx;

StringBuffer * strbuff_init( char *base_path );

void 		strbuff_destroy( StringBuffer *buf );

//char *			getFullPath( StringBuffer *buf, char *relative_path );

// add (...) multi arguments
char *strbuff_setFullPath( StringBuffer *buf, const char *relative_path );

char *			strbuff_addPostfix( StringBuffer *buf, const char *postfix );


StrBuff_base_idx strbuff_getBaseIdx( StringBuffer *buf );
StrBuff_base_idx strbuff_resetRelativePath( StringBuffer *buf );
void 			 strbuff_setRelativePath( StringBuffer *buf, StrBuff_base_idx relative_idx );

char * strbuff_getPathWithBase( StringBuffer *buf, StrBuff_base_idx start_from );
char * strbuff_getRelativeStr( StringBuffer *buf );

void 	setNewBase( StringBuffer *buf, char *new_base_path );

void 	strbuff_printBase( StringBuffer *buf, FILE *stream );

#endif // _DEV2FS_200MS_NET__LIB_STRING_BUFFER_H_
