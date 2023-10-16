/* hlmapping.h --- Header file for hlmapping.c.
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

/**
 * Backend
 */

#ifndef _TEAFS_ORG_FORMAT_HLMAPPING_H_
#define _TEAFS_ORG_FORMAT_HLMAPPING_H_

#include <stdio.h>
#include "../core/map.h"
#include "../lib/stringBuffer.h"

// file handler structure definiton
#include "../core/engine.h"


void hlmapping_mkDir( MapNode *map_node, MapNode *target_map_node, Tag *tag, StringBuffer *str_buff );
void hlmapping_rmDir( void *map_context, StringBuffer *str_buff );

int hlmapping_createFile( void *map_context, StringBuffer *str_buff,
											const char *file_name, const char *path, 
											int flags, mode_t mode, 
											struct file_handler *f_handler );

int hlmapping_unlinkFile( void *map_context, StringBuffer *str_buff, const char *file_name );

char * hlmapping_renameDirTest( MapNode *src_map_node );
//void inline hlmapping_renameDir( MapNode *src_map_node, MapNode *dest_map_node, char *src_file_name, char *dest_file_name );
int hlmapping_renameFile( void *map_context, StringBuffer *str_buff, const char *src_file_name, const char *dest_file_name );

#endif // _TEAFS_ORG_FORMAT_HLMAPPING_H_
