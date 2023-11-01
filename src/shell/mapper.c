/* mapper.c --- Creates the teaFS map of data from storage directory.
 * Copyright (C) 2012 Mateusz Piwek
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


void mapp_init( const struct loader *load, struct mapper *map )
{
	map->str_buff = strbuff_init( load->str_path );

}


void scan_source_directory( char *source_dir_path, struct mapper *map )
{
}

void map_release( struct mapper *map )
{

}
