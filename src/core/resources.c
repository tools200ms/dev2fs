/* resources.c --- ResourceMap manipulation functions used for teaFS's 
                   map building in shell/mapper.c.
 * Copyright (C) 2012 Mateusz Piwek
 * 
 * This file is part of Dev2FS.
 * 
 * Dev2FS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Dev2FS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Dev2FS.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "resources.h"

#include <stdio.h>
#include "../lib/messages.h"


// ResID NULL_ResID = {NULL};

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

}

/**
	Resource list
*/





