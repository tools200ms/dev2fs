/* map.h --- Defines the structures for holding teaFS map.
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


#ifndef _TEAFS_ORG_CORE_MAP_H_
#define _TEAFS_ORG_CORE_MAP_H_

#include "../lib/tags.h"

#include "../lib/list.h"


/**

Example of three containers configuration
______________     ______________
|Container 1 |     |Container 2 |     ______________
| tags:      |     |tags:       |     |Container 4 |
|   A <------|---->|   B <------|---->|tags:       |
|            |     --------------     |   C <------|--...
|            |     ______________     |   D <------|--...
|            |     |Container 3 |     |            |
|            |     |tags:       |     |            |
|   B <------|---->|   A <------|---->|            |
--------------     --------------     --------------

_Container_1_____    _Container_2_____    _Container_3_____    _Container_4_____
|_ref_from_____||    |_ref_from_____||    |_ref_from_____||    |_ref_from_____||
||   NULL      ||    ||A,container 1||    ||B,container 1||    ||A,container 3||
||             ||    ||             ||    ||             ||    ||B,container 2||
|---------------|    |---------------|    |---------------|    |---------------|
|_ref_to________|    |_ref_to________|    |_ref_to________|    |_ref_to________|
||A,container 2||    ||B,container 4||    ||A,container 4||    ||C,container..||
||B,container 3||    ||             ||    ||             ||    ||D,container..||
|---------------|    |---------------|    |---------------|    |---------------|
|ref_from_count:|    |ref_from_count:|    |ref_from_count:|    |ref_from_count:|
|0              |    |1              |    |1              |    |2              |
|tags:          |    |tags:          |    |tags:          |    |tags:          |
|   NULL        |    |A              |    |B              |    |AB             |
|int level:     |    |int level:     |    |int level:     |    |int level:     |
|0              |    |1              |    |1              |    |2              |
-----------------    -----------------    -----------------    -----------------

**/


typedef struct map_node
{
	/**
		redundant data to speedup some common queries
	*/
	int 		ref_from_count;
	short int 	level;

	/**
		context data to be passed to mapping
	*/
	void *context_data;
}
MapNode;

typedef struct map_reference
{
	/**
		tag defining the name of a next container OR 
		tag defining the name of the current container from 
		the perspective of the previous one
	*/
	Tag *tag;
	/**
		reference to the next container OR 
		reference to the previous container
	*/
	MapNode *sub_ref, *add_ref;

	//next or previous container
}
MapRef;

/*
	initializes empty node
*/
MapNode *map_init();
void map_destroy();

MapNode *map_newNode( MapNode *node, Tag *tag );
int map_attachNode( MapNode *node, MapNode *attaching_node, Tag *tag );

int map_removeNode( MapNode *node );
Tag **map_removeEmptyNode( MapNode *node );

void *map_setContext( MapNode *node, void *context );
void *map_getContext( MapNode *node );

MapNode *map_navigateAddTag( MapNode *node, Tag *tag );
MapNode *map_navigateSubTag( MapNode *node, Tag *tag );

MapNode *map_navigateSubAnyTag( MapNode *node, Tag **tag_ptr );

//int map_removeTag( NotTreeRef *not_tree_ref, Tag *tag );


#endif // _TEAFS_ORG_CORE_MAP_H_
