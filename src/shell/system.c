/* system.c --- Prepares FUSE for launch.
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
	//	defined in teafs.c
	operations->init = teafs_init;

	// assigns file system statistics operations
	operations->statfs		 = teaop_statfs;

	// assigns directory read operations

	operations->opendir		= teaop_opendir;
	operations->readdir		= teaop_readdir;
	operations->releasedir	= teaop_releasedir;

	// assigns directory modify operations
	operations->mkdir			 = teaop_mkdir;
	operations->rmdir			 = teaop_rmdir;

	// assign file read operations

	operations->open			 = teaop_open;
	operations->read			 = teaop_read;

	operations->release		 = teaop_release;

	// assign file modify operations

	operations->create		 = teaop_create;
	operations->write			 = teaop_write;

	operations->truncate		 = teaop_truncate;
	operations->flush			 = teaop_flush;
	operations->unlink		 = teaop_unlink;

	// assigns file and directory modify operations
	operations->rename		 = teaop_rename;
	operations->chmod		 = teaop_chmod;

	// assigns attribute read operations
	operations->getattr		 = teaop_getattr;
	operations->fgetattr	= teaop_fgetattr;

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

