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
	// Pre-check:
	#ifdef DEBUG
		/**
		  * .fh variable defined in 'struct fuse_file_info fi' is
		  * used to share 'struct file_handler' and
		  * 'struct dir_handler' beatween fuse fs operations
		  * 'calls.
		  * 'Ensure that file_handler and dir_handler fits 'fi.fh'
		  */
		struct fuse_file_info fi;

		if( sizeof( fi.fh ) < sizeof (struct file_handler) ||
			sizeof( fi.fh ) < sizeof (struct  dir_handler) ) {

			MSG_DEBUG_( "sizeof ( sizeof( fi.fh ) )", "%lu", sizeof (sizeof( fi.fh )) );
			MSG_DEBUG_( "sizeof ( struct file_handler )", "%lu", sizeof (struct file_handler) );
			MSG_DEBUG_( "sizeof ( struct dir_handler )", "%lu", sizeof (struct dir_handler) );
			MSG_ERROR_AND_EXIT("'struct file_handler' or 'struct dir_handler' does not fit into 'struct fuse_file_info .fh'");
		}
	#endif

	struct fuse_operations *op = (*sys).operations = malloc( sizeof (struct fuse_operations) );

	memset( op, 0, sizeof (struct fuse_operations) );

	// assigns init and terminate functions - declared in operations.h,
	//	defined in dev2fs.c
	op->init = dev2fs_init;

	// assigns file system statistics operations
	op->statfs		 = d2op_statfs;

	// assigns directory read operations
	op->opendir		= d2op_opendir;
	op->readdir		= d2op_readdir;

	op->releasedir	= d2op_releasedir;

	// assigns directory modify operations
	op->mkdir		= d2op_mkdir;
	op->rmdir		= d2op_rmdir;

	// assign file read operations
	op->open		= d2op_open;
	op->read		= d2op_read;

	op->release		= d2op_release;

	// assign file modify operations
	op->create		 = d2op_create;
	op->write		 = d2op_write;

	op->truncate	 = d2op_truncate;
	op->unlink		 = d2op_unlink;

	op->rename		 = d2op_rename;
	op->mknod		 = d2op_mknod;

	// assigns attribute read & modify operations
	op->getattr 	= d2op_getattr;
	op->fgetattr 	= d2op_fgetattr;
	op->chmod 		= d2op_chmod;
	op->chown 		= d2op_chown;
	op->utimens 	= d2op_utimens;
	op->access		= d2op_access;


	// flush/sync operations
	op->flush		 = d2op_flush;
	op->fsync		 = d2op_fsync;
	op->fsyncdir	 = d2op_fsyncdir;

	// link read/modify operations:
	op->readlink   = d2op_readlink;
	op->symlink    = d2op_symlink;
	op->link       = d2op_link;


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

