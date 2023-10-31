/* mapper.h --- Header file for mapper.c.
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


#ifndef _TEAFS_ORG_SHELL_MAPPER_H_
#define _TEAFS_ORG_SHELL_MAPPER_H_

#include "config.h"
#include "loader.h"

#include "../lib/stringBuffer.h"

#include "../core/map.h"


struct mapper
{
	MapNode *map_blank_node;
	
	StringBuffer *str_buff;
};

void mapp_init( const struct loader *load, struct mapper *map );

void map_release( struct mapper *map );

#endif // _TEAFS_ORG_SHELL_MAPPER_H_
