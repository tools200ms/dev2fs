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

	map_node->addTag_ref = rdxTrie_init();
	
	return map_node;
}

static MapNode *map_createNode()
{
	MapNode *node = malloc( sizeof (MapNode) );

	memset( node, 0, sizeof (MapNode) );

	node->addTag_ref = rdxTrie_init();
	node->subTag_ref = rdxTrie_init();

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
	ValueWrapper *val_wrap;
	
	val_wrap = rdxTrie_insert( node->addTag_ref, tag );
	
	if( val_wrap->value_ptr == NULL )
	{
		MapNode *add_node = map_createNode();
		
		MapRef *map_ref = map_initMapRef( tag, node, add_node );
		
		/* bind map nodes by map reference */
		val_wrap->value_ptr = map_ref;
		
		val_wrap = rdxTrie_insert( add_node->subTag_ref, tag );
		
		val_wrap->value_ptr = map_ref;
		
		return add_node;
	}
	
	return NULL;
}

int map_attachNode( MapNode *node, MapNode *attaching_node, Tag *tag )
{
	ValueWrapper *val_wrap;
	
	val_wrap = rdxTrie_insert( node->addTag_ref, tag );
	
	if( val_wrap->value_ptr == NULL )
	{
		MapRef *map_ref = map_initMapRef( tag, node, attaching_node );
		
		/* bind map nodes by map reference */
		val_wrap->value_ptr = map_ref;
		
		val_wrap = rdxTrie_insert( attaching_node->subTag_ref, tag );
		
		val_wrap->value_ptr = map_ref;
		
		return 0;
	}
	
	return -1;
}

/*static void map_realeseReference( void *map_ref )
{
	MSG_DEBUG( 			"      removing", "%s", ((MapRef *)map_ref)->tag );
	rdxTrie_remove( (((MapRef *)map_ref)->sub_ref)->addTag_ref, ((MapRef *)map_ref)->tag );
	
	free( map_ref );
}*/

Tag **map_removeEmptyNode( MapNode *node )
{
	if( rdxTrie_count( node->addTag_ref ) != 0 )
		return NULL;
	
	int tags_count = rdxTrie_count( node->subTag_ref );
	MapRef **map_refs = malloc( sizeof (MapRef *) * tags_count );
	Tag **tags = malloc( sizeof (Tag *) * (tags_count + 1) );

	if( (map_refs == NULL) || (tags == NULL) )
		return NULL;
	
	tags[tags_count] = NULL;
	
	
	rdxTrie_destroy( node->addTag_ref, NULL );
	
	rdxTrie_copyToTable( node->subTag_ref, (void **)map_refs, tags_count );
	rdxTrie_destroy( node->subTag_ref, NULL );
	
	//rdxTrie_destroy( node->subTag_ref, map_realeseReference );

	for( --tags_count; tags_count != -1; --tags_count )
	{
		rdxTrie_remove( ((map_refs[tags_count])->sub_ref)->addTag_ref, tags[tags_count] = ((map_refs[tags_count])->tag) );
		free( map_refs[tags_count] );
		MSG_DEBUG( 			"      removing", "%s", tags[tags_count] );
	}
	
	free( node );
	free( map_refs );
	
	return tags;
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
	ValueWrapper *val_wrap;
	
	if( (val_wrap = rdxTrie_find( node->addTag_ref, tag )) != NULL )
		return ((MapRef *)(val_wrap->value_ptr))->add_ref;
	
	return NULL;
}

MapNode *map_navigateSubTag( MapNode *node, Tag *tag )
{
	if( node->subTag_ref == NULL ) // blank node
		return NULL;
	
	ValueWrapper *val_wrap;
	
	if( (val_wrap = rdxTrie_find( node->subTag_ref, tag )) != NULL )
		return ((MapRef *)(val_wrap->value_ptr))->sub_ref;
	
	return NULL;
}

MapNode *map_navigateSubAnyTag( MapNode *node, Tag **tag_ptr )
{
	if( node->subTag_ref == NULL ) // blank node
		return NULL;
	
	ValueWrapper *val_wrap;
	
	val_wrap = rdxTrie_findByPosition( node->subTag_ref, 0 );
	
	*tag_ptr = ((MapRef *)(val_wrap->value_ptr))->tag;
	MSG_DEBUG_STR( "   val_wrap->value_ptr: ", ((MapRef *)(val_wrap->value_ptr))->tag );
	MSG_DEBUG_DEC( "   val_wrap->value_ptr: ", ((MapRef *)(val_wrap->value_ptr))->sub_ref );
	return ((MapRef *)(val_wrap->value_ptr))->sub_ref;
}

