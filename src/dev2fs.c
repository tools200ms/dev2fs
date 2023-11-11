/* dev2fs.c --- Calls all necessary functions to launch and terminate program.
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


#include <stdlib.h>
#include <string.h>

/* FUSE system */
#include "dev2fs.h"
#include <fuse.h>
/* *********** */

/* messages printing */
#include <stdio.h>
#include "lib/messages.h"
/* ***************** */

/* reflects program launching stages */
#include "shell/config.h"
#include "shell/loader.h"
#include "shell/mapper.h"
#include "shell/system.h"

//#include "core/operations.h"
/* ********************************* */

// include 'major' function
#include <sys/sysmacros.h>


struct dev2fs_references
{
	Config *conf;
	struct loader *load;
	struct mapper *map;
	struct system *sys;
}
d2_ref;

void *dev2fs_init( struct fuse_conn_info *conn )
{
	MSG_VERBOSE( "Dev2FS: OK, up and running, awaiting fs calls" );
	return NULL;
}

void teafs_terminate()
{
	if( d2_ref.sys != NULL )
	{
		sys_release( d2_ref.sys );
		free( d2_ref.sys );
	}

	if( d2_ref.map != NULL )
	{
		map_release( d2_ref.map );
		free( d2_ref.map );
	}

	//load_release();
	if( d2_ref.load != NULL )
	{
		load_release( d2_ref.load );
		free( d2_ref.load );
	}

	if( d2_ref.conf != NULL )
	{
		conf_destroy( d2_ref.conf );
	}
}

int main( int argc, char *argv[] )
{
	int ret_val = 0;

	memset( &d2_ref, 0, sizeof( struct dev2fs_references ) );

	/**
	 *	teaFS configuration
	*/

	if( (d2_ref.conf = conf_init( argc, argv )) == NULL )
		return -1;

	//add handler to call at exit

	if( (d2_ref.load = malloc( sizeof (struct loader) )) == NULL )
		return 1;
	memset( d2_ref.load, 0, sizeof (struct loader) );

	load_init( d2_ref.conf, d2_ref.load );
	MSG_VERBOSE( "CONFIG: OK, configuration successfully loaded" );

	MSG_VERBOSE_FUN_ARGS( loadedconf_print_summary, &((*(d2_ref.conf)).data), d2_ref.load );

	if( (d2_ref.map = malloc( sizeof (struct mapper) )) == NULL )
		return 1;

	memset( d2_ref.map, 0, sizeof (struct mapper) );

	//MSG_VERBOSE( "Checking source directory" );
	mapp_init( d2_ref.load, d2_ref.map );

	MSG_VERBOSE( "DATA SOURCE: OK, source directory tagged and mapped" );

	//"no integity check parformed, it's advised to run with -C"

	if( (d2_ref.sys = malloc( sizeof (struct system) )) == NULL )
		return 1;
	memset( d2_ref.sys, 0, sizeof (struct system) );

	sys_init( d2_ref.load, d2_ref.map, d2_ref.sys );


	MSG_VERBOSE( "DEV2FS: Starting FUSE" );
	ret_val = fuse_main( 	(*((*(d2_ref.conf)).fuse_arguments)).argc,
									(*((*(d2_ref.conf)).fuse_arguments)).argv,
									(d2_ref.sys)->operations,
									NULL );

	// TODO add signals handler and call it when exit signal comes !!!

	return ret_val;
}
