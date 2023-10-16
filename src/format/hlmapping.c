/* hlmapping.c --- Hard-linking storage format functions.
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


/* messages printing */
#include <stdio.h>
#include "../lib/messages.h"
/* ***************** */

/* filesystem, files and directories operations */
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
/* ******************************************** */

/* string manipulation functions */
#include <string.h>
/*********************************/

#include "hlmapping.h"
#include "../core/resources.h"


/* private function declarations */
static void hlmapping_copyTreeBranch_print( void *map_ref, void *str_buff );
static void hlmapping_copyTreeBranch( MapNode *next_map_node, StringBuffer *str_buff );
/* ***************************** */


static void hlmapping_copyTreeBranch_print( void *map_ref, void *str_buff )
{
	strbuff_setFullPath( (StringBuffer *)str_buff, ((MapRef *)map_ref)->tag );

	ResLocations *location = map_getContext( ((MapRef *)map_ref)->add_ref );
	char *prev_location = location->paths[ (location->last_el) - 1 ];

	char *buf = strdup( strbuff_getPathWithBase( (StringBuffer *)str_buff, 0 ) );
	int prefix_len = strlen( buf ) - strlen( prev_location );
	char *new_location = strdup( buf + prefix_len );
	strcpy( buf + prefix_len, prev_location );
	
	char *src;
	MSG_DEBUG( "    !!!         ", "%s", strbuff_getRelativeStr( (StringBuffer *)str_buff ) );

	MSG_DEBUG( "   buf", "%s", buf );
	MSG_DEBUG( "   new_location", "%s", new_location );
	MSG_DEBUG( "   prev_location", "%s", prev_location );
	MSG_DEBUG( "   location->last_el", "%d", location->last_el );

	locat_add( location, new_location );
	mkdir( strbuff_getPathWithBase( str_buff, 0 ), S_IRWXU | S_IRWXG );
	
	// copy files
	DIR *source_dir_ptr;
	struct dirent *dir_entry;
	struct stat statbuf;
	
	if( (source_dir_ptr = opendir( buf )) == NULL )
	{
		perror( msg_getProgramName() );
	}

//	dest_dir_ptr

	strbuff_addPostfix( str_buff, "/" );
	
	StrBuff_base_idx base_idx = strbuff_getBaseIdx( str_buff );
	strbuff_resetRelativePath( str_buff );
	
	while( (dir_entry = readdir( source_dir_ptr )) != NULL )
	{
		strbuff_setFullPath( str_buff, dir_entry->d_name );
		
		src = strdup( strbuff_getPathWithBase( str_buff, 0 ) );
		strcpy(src + prefix_len, prev_location );
		*(src + strlen( src )) = '/';
		
		if( lstat( src, &statbuf ) == -1 )
			perror( msg_getProgramName() );
		
		MSG_DEBUG( "  dest PATH ", "%s", strbuff_getPathWithBase( str_buff, 0 ) );
		MSG_DEBUG( "  src  PATH ", "%s", src );
		
		if( S_ISREG( statbuf.st_mode ) )
		{
			MSG_DEBUG( "   dest FILE", "%s", strbuff_getPathWithBase( str_buff, 0 ) );
			
			if( link( src, strbuff_getPathWithBase( str_buff, 0 ) ) == -1 )
				perror( msg_getProgramName() );
			//MSG_DEBUG( "   DESTIONATION FILE", "%s", dest );
		}
		
		free( src );
	}

	strbuff_setRelativePath( str_buff, base_idx );
	
	closedir( source_dir_ptr );
	free( buf );

	MSG_DEBUG( "    !!!         ", "%s", strbuff_getRelativeStr( (StringBuffer *)str_buff ) );

	
	hlmapping_copyTreeBranch( ((MapRef *)map_ref)->add_ref, (StringBuffer *)str_buff );
}

static void hlmapping_copyTreeBranch( MapNode *next_map_node, StringBuffer *str_buff )
{
	StrBuff_base_idx relative_idx = strbuff_getBaseIdx( str_buff );
	
	strbuff_addPostfix( str_buff, "/" );
	
	strbuff_resetRelativePath( str_buff );
	
	rdxTrie_print( next_map_node->addTag_ref, hlmapping_copyTreeBranch_print, str_buff );
	
	strbuff_setRelativePath( str_buff, relative_idx );
}

void hlmapping_mkDir( MapNode *map_node, MapNode *target_map_node, Tag *tag, StringBuffer *str_buff )
{
	int i, new_node;
	ResLocations *target_locations, *locations;
	char *src_dir = NULL;

	/********************/
	DIR *source_dir_ptr;
	struct dirent *dir_entry;
	struct stat statbuf;
	long dir_list_start;
	int file_name_start, src_file_name_start;
	char src_buf[2048], buf[2048];
	/********************/


	if( (new_node = ((target_locations = map_getContext( target_map_node )) == NULL)) )
	{ // new node
		map_setContext( target_map_node, target_locations = locat_init() );
	}


	locations = map_getContext( map_node );
	MSG_DEBUG( "locations", "%d", locations );
	MSG_DEBUG( "locations", "%d", locations->last_el );
	
	StrBuff_base_idx relative_idx = strbuff_getBaseIdx( str_buff );
	
	for( i = 0; i != locations->last_el; ++i )
	{
		strbuff_setFullPath( str_buff, locations->paths[i] );
		strbuff_addPostfix( str_buff, "/" );
		strbuff_addPostfix( str_buff, tag );
		
		MSG_DEBUG( "MKDIR", "%s", strbuff_getPathWithBase( str_buff, 0 ) );
		mkdir( strbuff_getPathWithBase( str_buff, 0 ), S_IRWXU | S_IRWXG );
		
		locat_add( target_locations, strdup( strbuff_getRelativeStr( str_buff ) ) );
		
		MSG_DEBUG( "location path", "%s", strbuff_getRelativeStr( str_buff ) );


		if( !new_node )
		{
			if( src_dir == NULL )
			{
				src_dir = strdup( strbuff_getPathWithBase( (StringBuffer *)str_buff, 0 ) );
				int prefix_len = strlen( src_dir ) - strlen( target_locations->paths[ 0 ] );
			//char *new_location = strdup( buf + prefix_len );
				strcpy( src_dir + prefix_len, target_locations->paths[ 0 ] );
				
				//MSG_DEBUG( "   locations->paths[ 0 ]", "%s", target_locations->paths[ 0 ] );
				//MSG_DEBUG( "   strbuff_getPathWithBase( (StringBuffer *)str_buff, 0 )", "%s", strbuff_getPathWithBase( (StringBuffer *)str_buff, 0 ) );
				//MSG_DEBUG( "   src_dir", "%s", src_dir );
				if( (source_dir_ptr = opendir( src_dir )) == NULL )
					perror( msg_getProgramName() );
				
				dir_list_start = telldir( source_dir_ptr );
				
				strcpy( src_buf, src_dir );
				strcat( src_buf, "/" );
				src_file_name_start = strlen( src_buf );
			}
			else
				seekdir( source_dir_ptr, dir_list_start );
			
			
			strcpy( buf, strbuff_getPathWithBase( (StringBuffer *)str_buff, 0 ) );
			strcat( buf, "/" );
			file_name_start = strlen( buf );
			
			while( (dir_entry = readdir( source_dir_ptr )) != NULL )
			{
				strcpy( src_buf + src_file_name_start, dir_entry->d_name );
				
				if( lstat( src_buf, &statbuf ) == -1 )
					perror( msg_getProgramName() );
				
				if( S_ISREG( statbuf.st_mode ) )
				{
					strcpy( buf + file_name_start, dir_entry->d_name );
					
					MSG_DEBUG( "    SRC DIR", "%s", src_buf );
					MSG_DEBUG( "        DIR", "%s", buf );
					if( link( src_buf, buf ) == -1 )
						perror( msg_getProgramName() );
				}
			}

			hlmapping_copyTreeBranch( target_map_node, str_buff );
			strbuff_setRelativePath( str_buff, relative_idx );
		}
	}
	
	if( src_dir != NULL )
	{
		closedir( source_dir_ptr );
		free( src_dir );
	}
}

void hlmapping_rmDir( void *map_context, StringBuffer *str_buff )
{
	int i, ret_val = 0;
	
	for( i = 0; i != ((ResLocations *)map_context)->last_el; ++i )
	{
		strbuff_setFullPath( str_buff, ((ResLocations *)map_context)->paths[i] );
		
		MSG_DEBUG( "    RMDIR", "%s", strbuff_getPathWithBase( str_buff, 0 ) );
		if( (ret_val = rmdir( strbuff_getPathWithBase( str_buff, 0 ) )) == -1 )
			perror( msg_getProgramName() );
	}
	
	locat_destroy( (ResLocations *)map_context );
}

int hlmapping_createFile( void *map_context, StringBuffer *str_buff,
											const char *file_name, const char *path, 
											int flags, mode_t mode, 
											struct file_handler *f_handler )
{
	int i;
	char *link_path;
	
	strbuff_setFullPath( str_buff, path );
	
	link_path = strdup( strbuff_getPathWithBase( str_buff, 0 ) );
	
	if( (f_handler->fd = open( link_path, flags, mode )) == -1 )
	{
		perror( msg_getProgramName() );
		return -1;
	}

	for( i = 0; i != ((ResLocations *)map_context)->last_el; ++i )
	{
		strbuff_setFullPath( str_buff, ((ResLocations *)map_context)->paths[i] );
		strbuff_addPostfix( str_buff, "/" );
		strbuff_addPostfix( str_buff, file_name );
		
		MSG_DEBUG( "\t=====> ", "%s", strbuff_getRelativeStr( str_buff ) );
		MSG_DEBUG( "\t=====> ", "%s", path );
		MSG_DEBUG( "\t=====> ", "%d", strcmp( strbuff_getRelativeStr( str_buff ), path ) );
		
		if( strcmp( strbuff_getRelativeStr( str_buff ), path ) != 0 )
		{
			MSG_DEBUG( "    CREATE FILE", "%s", strbuff_getPathWithBase( str_buff, 0 ) );
			
			if( link( link_path, strbuff_getPathWithBase( str_buff, 0 ) ) == -1 )
				perror( msg_getProgramName() );
		}
		//else
		//{
		//	MSG_DEBUG( "    CREATE FILE FOUND", "%s", link_path );
			//MSG_DEBUG_INFO( "found ..." );
		//}
	}
	
	free( link_path );
	
	return 0;
}

int hlmapping_unlinkFile( void *map_context, StringBuffer *str_buff, const char *file_name )
{
	int i, ret_val = 0;

	MSG_DEBUG( "\t=====> ", "%d", ((ResLocations *)map_context)->last_el );

	for( i = 0; i != ((ResLocations *)map_context)->last_el; ++i )
	{
		strbuff_setFullPath( str_buff, ((ResLocations *)map_context)->paths[i] );
		strbuff_addPostfix( str_buff, "/" );
		strbuff_addPostfix( str_buff, file_name );
		
		MSG_DEBUG( "\t=====> ", "%s", strbuff_getPathWithBase( str_buff, 0 ) );
		
		if( unlink( strbuff_getPathWithBase( str_buff, 0 ) ) == -1 )
		{
			perror( msg_getProgramName() );
			ret_val = -1;
		}
	}
	
	return ret_val;
}

char * hlmapping_renameDirTest( MapNode *src_map_node )
{
	Tag *src_tag;
	
	if( rdxTrie_count( src_map_node->subTag_ref ) != 1 )
		return NULL;
	
	map_navigateSubAnyTag( src_map_node, &src_tag );
	
	return src_tag;
}

int hlmapping_renameFile( void *map_context, StringBuffer *str_buff, const char *src_file_name, const char *dest_file_name )
{
	int i, ret_val = 0;
	char *src_path;

	for( i = 0; i != ((ResLocations *)map_context)->last_el; ++i )
	{
		strbuff_setFullPath( str_buff, ((ResLocations *)map_context)->paths[i] );
		strbuff_addPostfix( str_buff, "/" );
		strbuff_addPostfix( str_buff, src_file_name );
		
		src_path = strdup( strbuff_getPathWithBase( str_buff, 0 ) );
		
		strbuff_setFullPath( str_buff, ((ResLocations *)map_context)->paths[i] );
		strbuff_addPostfix( str_buff, "/" );
		strbuff_addPostfix( str_buff, dest_file_name );
		
		MSG_DEBUG( "\t=====> ", "%s", src_path );
		MSG_DEBUG( "\t=====> ", "%s", strbuff_getPathWithBase( str_buff, 0 ) );
		
		if( rename( src_path, strbuff_getPathWithBase( str_buff, 0 ) ) == -1 )
		{
			perror( msg_getProgramName() );
			ret_val = -1;
		}
		
		free( src_path );
	}
	
	return ret_val;
}

