/* resources.c --- ResourceMap manipulation functions used for teaFS's 
                   map building in shell/mapper.c.
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
#include <stdlib.h>
#include <string.h>

#include "resources.h"

#include <stdio.h>
#include "../lib/messages.h"


ResID NULL_ResID = {NULL};

/**
	Locations list
*/

ResLocations *locat_init()
{
	ResLocations *res_loc = malloc( sizeof (ResLocations) );
	
	if( res_loc == NULL )
		return NULL;
	
	res_loc->last_el = 0;
	
	return res_loc;
}

/*
	releases path allocated by mapper
*/
void locat_destroy( ResLocations *res_loc )
{
	int i;
	
	for( i = 0; i < res_loc->last_el; ++i )
	{
		free( res_loc->paths[i] );
	}
	
	free( res_loc );
}

void locat_add( ResLocations *res_loc, char *path )
{
	res_loc->paths[ (res_loc->last_el)++ ] = path;
	MSG_DEBUG( "NEW path", "%s", path );
	MSG_DEBUG( "      ID", "%d", res_loc );
}

/**
	Resource list
*/

ResourceMap *res_init()
{
	ResourceMap *rm = malloc( sizeof (ResourceMap) );

	if( rm == NULL )
		return NULL;
	
	rm->last_el = 0;
	
	return rm;
}

void res_destroy( ResourceMap *rm )
{
	int i;
	for( i = 0; i < rm->last_el; ++i )
	{
		free( rm->res_id[i] );
		//locat_destroy( rm->res_loc[i] );
		// TODO: realase locations
	}
	
	free( rm );
}

ResID *res_create( ResID *res_id, Tag *tag )
{
	ResID *new_res_id;
	int i, idx;

	for( i = 0, idx = 0; res_id[i] != NULL; ++i, ++idx )
	{
		if( tag < res_id[i] )
		{
			for( ; res_id[i] != NULL; ++i );
			break;
		}
		else if( tag == res_id[i] )
			return NULL;
	}

	new_res_id = malloc( sizeof (ResID) * (i + 2) );

	for( i = 0; 1 ; ++i, ++res_id )
	{
		if( i == idx )
			new_res_id[i++] = tag;
		
		if( (new_res_id[i] = *res_id) == NULL )
			break;
	}

	return new_res_id;
}

// addLocation
ResID *res_register( ResourceMap *res_map, ResID *res_id, MapNode ***map_node )
{
	int i, j;

	for( i = 0; i < res_map->last_el; ++i )
	{
		for( j = 0; res_id[j] != NULL; ++j )
		{
			if( res_id[j] != (res_map->res_id[i][j]) )
				break;
		}
		
		if( (res_id[j] == NULL) && (res_map->res_id[i][j] == NULL) )
		{
			free( res_id );
			
			//res_map->map_ref[ i ] = map_node; // = res_map->res_loc[i];
			*map_node = (res_map->map_ref) + i;
			
			return res_map->res_id[i];
		}
	}


	(res_map->res_id[ i ]) = res_id;
	(res_map->last_el)++;


	//res_map->map_ref[ i ] = map_node;
	*map_node = (res_map->map_ref) + i;
	**map_node = NULL;

	//*res_loc_buff = res_map->res_loc[i] = locat_init();
	return res_id;
}

// 
ResID *res_find( ResourceMap *res_map, ResID *res_id, Tag *tag )
{
	int i, j;
	res_id = res_create( res_id, tag );
	
	for( i = 0; i < res_map->last_el; ++i )
	{
		for( j = 0; res_id[j] != NULL; ++j )
		{
			if( res_id[j] != (res_map->res_id[i][j]) )
				break;
		}
		
		if( res_id[j] == NULL )
		{
			free( res_id );
			
			return res_map->res_id[i];
		}
	}
	
	return NULL;
}


MapNode *res_findMapNode( ResourceMap *res_map, ResID *res_id, Tag *tag )
{
	int i, j;
	res_id = res_create( res_id, tag );
	
	for( i = 0; i < res_map->last_el; ++i )
	{
		for( j = 0; res_id[j] != NULL; ++j )
		{
			if( res_id[j] != (res_map->res_id[i][j]) )
				break;
		}
		
		if( res_id[j] == NULL )
		{
			free( res_id );
			
			return res_map->map_ref[i];
		}
	}
	
	return NULL;
}

int res_removeByMapNode( ResourceMap *res_map, MapNode *node )
{
	int i;

	for( i = 0; i < (res_map->last_el); ++i )
	{
		if( res_map->map_ref[i] == node )
		{
			--(res_map->last_el);
			free( res_map->res_id[i] );
			
			res_map->res_id[i] = res_map->res_id[res_map->last_el];
			res_map->res_id[res_map->last_el] = NULL;
			
			res_map->map_ref[i] = res_map->map_ref[res_map->last_el];
			res_map->map_ref[res_map->last_el] = NULL;
			
			return 0;
		}
	}
	
	return -1;
}

int res_isTagUsed( ResourceMap *res_map, Tag *tag )
{
	int i;
	ResID *r_id;
//	Tag *t;
	
	for( i = 0; i < (res_map->last_el); ++i )
	{
		for( r_id = res_map->res_id[i]; *r_id != NULL; ++r_id )
		{
			if( *r_id == tag )
				return 1;
			else if( *r_id > tag )
				break;
		}
	}
	
	return 0;
}

void res_print( FILE *stream, ResourceMap *res_map )
{
	int i, j;
	ResLocations *res_loc;
	fprintf( stream, MSG_RESMAP_INFO );

	for( i = 0; i < (res_map->last_el); ++i )
	{
		if( (res_map->res_id[i][0]) != NULL )
		{
			fprintf( stream, "resource (%d): /%s/", i, res_map->res_id[i][0] );
			
			for( j = 1; res_map->res_id[i][j] != NULL; ++j )
			{
				fprintf( stream, ", " );
				fprintf( stream, "/%s/", res_map->res_id[i][j] );
			}
		}
		
		fprintf( stream, "\n" );
		
		res_loc = res_map->map_ref[i]->context_data;
		
		for( j = 0; j < res_loc->last_el ; ++j )
		{
			fprintf( stream, "(%d) %s\n", i, res_loc->paths[j] );
		}
		
		fprintf( stream, "\n" );
	}
}

