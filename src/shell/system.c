/* system.c --- Prepares FUSE for launch.
 * Copyright (C) 2012, 2023 Mateusz Piwek
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

/* FUSE system */
#include "../dev2fs.h"
#include <fuse.h>
/* *********** */

#include "config.h"
#include "mapper.h"
#include "system.h"


#include "../core/operations.h"
#include "../core/engine.h"


void sys_init( 	const struct config *conf, 
				const struct loader *load,
				const struct mapper *map,
					struct system *sys 			)
{
	struct fuse_operations *operations = operations = 
								malloc( sizeof (struct fuse_operations) );

	memset( operations, 0, sizeof (struct fuse_operations) );
	(*sys).operations = operations;

	// assigns init and terminate functions - declared in operations.h,
	//	defined in dev2fs.c
	operations->init = dev2fs_init;

	// assigns file system statistics operations
	operations->statfs		 = d2op_statfs;

	// assigns directory read operations
	operations->opendir		= d2op_opendir;
	operations->readdir		= d2op_readdir;

	operations->releasedir	= d2op_releasedir;

	// assigns directory modify operations
	operations->mkdir		= d2op_mkdir;
	operations->rmdir		= d2op_rmdir;

	// assign file read operations
	operations->open		= d2op_open;
	operations->read		= d2op_read;

	operations->release		= d2op_release;

	// assign file modify operations
	operations->create		 = d2op_create;
	operations->write		 = d2op_write;

	operations->truncate	 = d2op_truncate;
	operations->flush		 = d2op_flush;
	operations->unlink		 = d2op_unlink;

	operations->rename		 = d2op_rename;
	//

	// assigns attribute read & modify operations
	operations->getattr 	= d2op_getattr;
	operations->fgetattr 	= d2op_fgetattr;
	operations->chmod 		= d2op_chmod;
	operations->chown 		= d2op_chown;
	operations->utimens 	= d2op_utimens;


	// assigns attribute modify operations
	// assigns link read operations
	// assigns link modify operations


	//engine_setup( NULL, map->map_blank_node );
	operations_init( load, map->str_buff );
}

void sys_release( struct system *sys )
{
	if( sys->operations != NULL )
		free( sys->operations );
}

