/* resources.h --- Definitions for ResourceMap used as helper during
                   teaFS map building process in shell/mapper.c.
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


#ifndef _TEAFS_ORG_CORE_RESOURCES_H_
#define _TEAFS_ORG_CORE_RESOURCES_H_

#include "map.h"

/**
	resource declarations
*/

#define RES_MAX_COUNT 128
typedef struct resource_loacations
{
	char *paths[RES_MAX_COUNT];
	short int last_el;
}
ResLocations;

ResLocations *locat_init();
void locat_destroy( ResLocations *res_locat );

void locat_add( ResLocations *res_locat, char *path );

/**
	used during the map creation process
*/
// extern ResID NULL_ResID;

typedef struct resource_map
{
//	ResLocations *res_loc[256];
	MapNode *map_ref[256];

	int last_el;
}
ResourceMap;



#endif // _TEAFS_ORG_CORE_RESOURCES_H_
