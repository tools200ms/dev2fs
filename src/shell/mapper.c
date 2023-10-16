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
	scan_source_directory( (*load).source_dir_path, map );
	
	MSG_VERBOSE_FUN_ARGS( tags_print, map->tags_set );
	MSG_VERBOSE_FUN_ARGS( res_print, map->res_map );
}


void scan_source_directory( char *source_dir_path, struct mapper *map )
{/*
	RadixTrieNode *tags_set = rdxTrie_init();
	ResourceMap	 *res_map = res_init();
	MapNode *map_blank_node, *map_current_node, **map_new_node;
	map_blank_node = map_init();

	//Tag *tag;
	char *rel_path, *new_rel_path, *tag_str;
	ResID *res_id, *new_res_id;
	ResLocations *mapping_context;
	ValueWrapper *tag_val_warp;

	List 	*paths_list = init_list(), 
			*res_list = init_list(), 
			*map_nodes_list = init_list();
	DIR *dir_ptr;
	
	StrBuff_base_idx root_idx;
	short int base_path;


	struct dirent *dir_entry;
	struct stat dir_stat;

	StringBuffer *str_buff = strbuff_init( source_dir_path );
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

//	list_write( dirs_list, tag );
	list_write( paths_list, strdup( "" ) );
	list_write( res_list, &NULL_ResID );
	list_write( map_nodes_list, map_blank_node );

	map_setContext( map_blank_node, mapping_context = locat_init() );
	locat_add( mapping_context, strdup( "/." ) );

	while( !list_isEmpty( paths_list ) )
	{
		strbuff_setRelativePath( str_buff, base_path );
		
		rel_path = list_read( paths_list );
		res_id = list_read( res_list );
		map_current_node = list_read( map_nodes_list );
		
		MSG_DEBUG("SCANNING", "%s", rel_path );
		strbuff_setFullPath( str_buff, rel_path );

		//free( rel_path );
		strbuff_resetRelativePath( str_buff );
		
		if( (dir_ptr = opendir( str_buff->path_buff )) == NULL )
		{
			perror( msg_getProgramName() );
			return;
		}

		while( (dir_entry = readdir( dir_ptr )) != NULL )
		{
			strbuff_setFullPath( str_buff, "/" );
	
			strbuff_addPostfix( str_buff, dir_entry->d_name );
			
			if(	strcmp( ".", dir_entry->d_name ) == 0 ||
					strcmp( "..", dir_entry->d_name ) == 0		)
					continue;
			
			lstat( str_buff->path_buff, &dir_stat );

			if( S_ISDIR( dir_stat.st_mode ) )
			{
				list_write( paths_list, 
								new_rel_path = strdup( strbuff_getPathWithBase( str_buff, base_path ) ) );
				
				
				if( (tag_val_warp = rdxTrie_insert( tags_set, dir_entry->d_name ))->value_ptr == NULL )
				{
				// first occurrence of a tag
					
					tag_val_warp->value_ptr = strdup( dir_entry->d_name );
					
					//rdxTrie_insert( tags_set );
					//tags_print( stdout, tags_set );
				}
				//else
				//{
				//	MSG_DEBUG_STR( "OLD tag", tag_val_warp->value_ptr );
				//}
				
				tag_str = tag_val_warp->value_ptr;
				//tag = tags_add( tags_list, dir_entry->d_name );
				
				new_res_id = res_create( res_id, tag_str );
				if( new_res_id == NULL ) // double tag!
				{
					//print error
				}
				
				new_res_id = res_register( res_map, new_res_id, &map_new_node );
				
				
				if( *map_new_node == NULL ) // new resource, thus new map node too
				{
					*map_new_node = map_newNode( map_current_node, tag_str );
					//MSG_DEBUG_DEC( "map_new_node (NULL)", *map_new_node );
					
					map_setContext( *map_new_node, mapping_context = locat_init() );
				}
				else // merge tree branches
				{
					map_attachNode( map_current_node, *map_new_node, tag_str );
					
					mapping_context = map_getContext( *map_new_node );
				}
				
				MSG_DEBUG_STR("new_rel_path", new_rel_path );
				MSG_DEBUG_DEC("mapping_context", mapping_context );
				
				locat_add( mapping_context, new_rel_path );
				
				list_write( res_list, new_res_id );
				list_write( map_nodes_list, *map_new_node );
				
				MSG_DEBUG_BR;
				MSG_DEBUG_INFO( dir_entry->d_name );
				//MSG_DEBUG_INFO( pb->path_buff + base_path );
				MSG_DEBUG_BR;
			}
		}
		
		closedir( dir_ptr );
	}

	strbuff_setRelativePath( str_buff, root_idx );

	map->tags_set = tags_set;
	map->res_map = res_map;
	map->map_blank_node = map_blank_node;
	
	map->str_buff = str_buff;
	
	destroy_list( paths_list );
	destroy_list( res_list );
	destroy_list( map_nodes_list );*/
}

void map_release( struct mapper *map )
{
	res_destroy( map->res_map );
	rdxTrie_destroy( map->tags_set, free );
	// TODO: destroy map
	// TODO: destroy locations
	
	strbuff_destroy( map->str_buff );
}
