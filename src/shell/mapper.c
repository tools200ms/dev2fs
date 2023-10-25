/* mapper.c --- Creates the teaFS map of data from storage directory.
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


#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../lib/messages.h"

#include "mapper.h"

#include "../lib/list.h"


void scan_source_directory( char *source_dir_path, struct mapper *map );


void mapp_init( const struct config *conf, const struct loader *load, struct mapper *map )
{
	char *rel_path, *new_rel_path, *tag_str;

	List 	*paths_list = init_list(),
			*res_list = init_list(),
			*map_nodes_list = init_list();
	DIR *dir_ptr;

	StrBuff_base_idx root_idx;
	short int base_path;


	struct dirent *dir_entry;
	struct stat dir_stat;

	StringBuffer *str_buff = strbuff_init( (*load).source_dir_path );

	root_idx = strbuff_getBaseIdx( str_buff );

	MSG_DEBUG("pb->path_buff", "%s", str_buff->path_buff );
	MSG_DEBUG("pb->relative_path", "%s", str_buff->relative_path );
	MSG_DEBUG("pb->relative_path_length", "%d", str_buff->relative_path_length );
	MSG_DEBUG("pb->relative_path_max_length", "%d", str_buff->relative_path_max_length );

	base_path = strbuff_resetRelativePath( str_buff );
	MSG_DEBUG("pb->path_buff", "%s", str_buff->path_buff );
	MSG_DEBUG("pb->relative_path", "%s", str_buff->relative_path );
	MSG_DEBUG("pb->relative_path_length", "%d", str_buff->relative_path_length );
	MSG_DEBUG("pb->relative_path_max_length", "%d", str_buff->relative_path_max_length );


}


void scan_source_directory( char *source_dir_path, struct mapper *map )
{
}

void map_release( struct mapper *map )
{

}
