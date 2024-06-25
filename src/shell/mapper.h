/* mapper.h --- Header file for mapper.c.
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


#ifndef _DEV2FS_200MS_NET__SHELL_MAPPER_H_
#define _DEV2FS_200MS_NET__SHELL_MAPPER_H_

#include "config.h"
#include "loader.h"

#include "../lib/stringBuffer.h"


struct mapper
{

	StringBuffer *str_buff;
};

void mapp_init( const struct loader *load, struct mapper *map );

void mapp_check( struct mapper *map );
void map_release( struct mapper *map );

#endif // _DEV2FS_200MS_NET__SHELL_MAPPER_H_
