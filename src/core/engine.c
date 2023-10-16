/* engine.c --- Provides functions for operations.c to operate on teaFS map.
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

/* messages printing */
#include <stdio.h>
#include "../lib/messages.h"
/* ***************** */

#include "../lib/tags.h"
#include "engine.h"

static RadixTrieNode *op_tags_set;
static MapNode *op_map_blank_node;


void engine_setup( RadixTrieNode *tags_set, MapNode *map_blank_node )
{
	op_tags_set = tags_set;
	op_map_blank_node = map_blank_node;
}

MapNode *engine_findNode( char *path_pos )
{
	MapNode *current_node = op_map_blank_node;
	Tag *tag_str;
	++path_pos;
	
	while( *path_pos != '\0' )
	{
		if( (tag_str = rdxTrie_findInPath( op_tags_set, path_pos, &path_pos )) == NULL )
			return NULL;// wrong path !
		
		//change node to
		if( (current_node = map_navigateAddTag( current_node, tag_str )) == NULL )
			return NULL;// no such tag in certain resource
	}

	return current_node;
}

MapNode *engine_findNodeAndExtractFileName( char *path_pos, char **file_name )
{
	MapNode *current_node = op_map_blank_node;
	char *next_path_pos;
	Tag *tag_str;
	++path_pos;
	
	find_tag:
	if( (tag_str = rdxTrie_findInPath( op_tags_set, path_pos, &next_path_pos )) != NULL )
	{
		//change node to
		if( (current_node = map_navigateAddTag( current_node, tag_str )) != NULL )
		{
			if( *next_path_pos != '\0' )
			{
				path_pos = next_path_pos;
				goto find_tag;
			}
			else
			{
				// no file name found in path
				*file_name = NULL;
				
				return current_node;
			}
		}
		// no such tag in certain node
	}
	else
	{
		// no such tag found, must be a file name
		if( *next_path_pos == '\0' )
		{
			*file_name = path_pos;
			
			return current_node;
		}
		// the path item is not the last item, seems like a wrong path was given
	}

	return NULL;
}

MapNode *engine_findNodeForNewDirectory( char *path_pos, Tag **tag )
{
	MapNode *current_node = op_map_blank_node, *next_node;
	char *next_path_pos;
	Tag *tag_str;
	++path_pos;
	
	find_tag:
	if( (tag_str = rdxTrie_findInPath( op_tags_set, path_pos, &next_path_pos )) != NULL )
	{
		//change node to
		if( (next_node = map_navigateAddTag( current_node, tag_str )) != NULL )
		{
			if( *next_path_pos != '\0' )
			{
				path_pos = next_path_pos;
				current_node = next_node;
				goto find_tag;
			}
			// last tag, no room for a new directory
		}
		else
		{
			if( *next_path_pos == '\0' )
			{ // found node to create new directory with existing tag
				
				*tag = tag_str;
				return current_node;
			}
		}
	}
	else
	{	// directory name is a NEW tag

		if( *next_path_pos == '\0' )
		{ // found node to create new directory with a new tag
			*tag = path_pos - 1;

			return current_node;
		}
	}

	return NULL;
}

