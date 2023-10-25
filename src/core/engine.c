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

static MapNode *op_map_blank_node;


void engine_setup( MapNode *map_blank_node )
{
	op_map_blank_node = map_blank_node;
}

MapNode *engine_findNode( char *path_pos )
{
	MapNode *current_node = op_map_blank_node;


	return current_node;
}

MapNode *engine_findNodeAndExtractFileName( char *path_pos, char **file_name )
{

	return NULL;
}

MapNode *engine_findNodeForNewDirectory( char *path_pos, Tag **tag )
{

	return NULL;
}

