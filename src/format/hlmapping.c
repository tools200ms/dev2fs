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

}

static void hlmapping_copyTreeBranch( MapNode *next_map_node, StringBuffer *str_buff )
{

}

void hlmapping_mkDir( MapNode *map_node, MapNode *target_map_node, Tag *tag, StringBuffer *str_buff )
{

}

void hlmapping_rmDir( void *map_context, StringBuffer *str_buff )
{

}

int hlmapping_createFile( void *map_context, StringBuffer *str_buff,
											const char *file_name, const char *path, 
											int flags, mode_t mode, 
											struct file_handler *f_handler )
{

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
	return NULL;
}

int hlmapping_renameFile( void *map_context, StringBuffer *str_buff, const char *src_file_name, const char *dest_file_name )
{
	return 0;
}

