/* map.c --- Functions to operate on teaFS map structures.
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

#include <stdlib.h>
#include <string.h>
#include "map.h"



MapNode *map_init()
{
	MapNode *map_node = malloc( sizeof (MapNode) );
	
	memset( map_node, 0, sizeof (MapNode) );


	
	return map_node;
}

static MapNode *map_createNode()
{
	MapNode *node = malloc( sizeof (MapNode) );

	memset( node, 0, sizeof (MapNode) );



	return node;
}

static MapRef *map_initMapRef( Tag *tag, MapNode *sub_node, MapNode *add_node )
{
	MapRef *map_ref = malloc( sizeof(MapRef) );
	map_ref->tag = tag;
	
	map_ref->sub_ref = sub_node;
	map_ref->add_ref = add_node;
	
	return map_ref;
}

void map_destroy()
{
	// set to hold uniquly references
}

// MapNode *map_addNewNode( MapNode *node, MapNode *new_node, Tag *tag )
MapNode *map_newNode( MapNode *node, Tag *tag )
{

	
	return NULL;
}

int map_attachNode( MapNode *node, MapNode *attaching_node, Tag *tag )
{

	
	return -1;
}



Tag **map_removeEmptyNode( MapNode *node )
{

	return NULL;
}

void *map_setContext( MapNode *node, void *context )
{
	void *prev_context_data = node->context_data;
	
	node->context_data = context;
	
	return prev_context_data;
}

void *map_getContext( MapNode *node )
{
	return node->context_data;
}

MapNode *map_navigateAddTag( MapNode *node, Tag *tag )
{

	return NULL;
}

MapNode *map_navigateSubTag( MapNode *node, Tag *tag )
{

	return NULL;
}

MapNode *map_navigateSubAnyTag( MapNode *node, Tag **tag_ptr )
{
	return NULL;
}

