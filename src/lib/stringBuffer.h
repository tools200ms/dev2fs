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

// #define REL_PATH_MAX_BUFFSIZE 4096
#define REL_PATH_DEFAULT_BUFFSIZE 128
#define REL_PATH_BUFF_MARGIN 64

typedef struct path_buffer
{
	char *p_buf, *rp_buf_start;
	size_t rp_buf_len;
}
StringBuffer;

typedef short int StrBuff_base_idx;

StringBuffer * strbuff_init( char *base_path );

void 		strbuff_destroy( StringBuffer *buf );

//char *			getFullPath( StringBuffer *buf, char *relative_path );

// add (...) multi arguments
char *strbuff_setFullPath(StringBuffer *buf, const char *relative_path);

void strbuff_release(char *path);


#endif // _DEV2FS_200MS_NET__LIB_STRING_BUFFER_H_
