/* teafs.c --- Calls all necessary functions to launch and terminate program.
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


#include <stdlib.h>
#include <string.h>

/* FUSE system */
#include "devfs.h"
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

struct teafs_references
{
	Config *conf;
	struct loader *load;
	struct mapper *map;
	struct system *sys;
}
tea_ref;

void *teafs_init( struct fuse_conn_info *conn )
{
	MSG_VERBOSE( "TEAFS: OK, up and running, awaiting fs calls" );
	return NULL;
}

void teafs_terminate()
{
	if( tea_ref.sys != NULL )
	{
		sys_release( tea_ref.sys );
		free( tea_ref.sys );
	}

	if( tea_ref.map != NULL )
	{
		map_release( tea_ref.map );
		free( tea_ref.map );
	}

	//load_release();
	if( tea_ref.load != NULL )
	{
		load_release( tea_ref.load );
		free( tea_ref.load );
	}

	if( tea_ref.conf != NULL )
	{
		conf_destroy( tea_ref.conf );
	}
}

int main( int argc, char *argv[] )
{
	int ret_val = 0;

	memset( &tea_ref, 0, sizeof( struct teafs_references ) );

	/**
	 *	teaFS configuration
	*/

	if( (tea_ref.conf = conf_init( argc, argv )) == NULL )
		return -1;

	MSG_VERBOSE_FUN_ARGS( conf_print_parameters, &((*(tea_ref.conf)).data) );
	//add handler to call at exit

	if( (tea_ref.load = malloc( sizeof (struct loader) )) == NULL )
		return 1;
	memset( tea_ref.load, 0, sizeof (struct loader) );

	load_init( tea_ref.conf, tea_ref.load );
	MSG_VERBOSE( "CONFIG: OK, configuration successfully loaded" );

	if( (tea_ref.map = malloc( sizeof (struct mapper) )) == NULL )
		return 1;

	memset( tea_ref.map, 0, sizeof (struct mapper) );

	//MSG_VERBOSE( "Checking source directory" );
	mapp_init( tea_ref.conf, tea_ref.load, tea_ref.map );

	MSG_VERBOSE( "DATA SOURCE: OK, source directory tagged and mapped" );

	//"no integity check parformed, it's advised to run with -C"

	if( (tea_ref.sys = malloc( sizeof (struct system) )) == NULL )
		return 1;
	memset( tea_ref.sys, 0, sizeof (struct system) );

	sys_init( tea_ref.conf, tea_ref.load, tea_ref.map, tea_ref.sys );


	MSG_VERBOSE( "TEAFS: Starting FUSE" );
	ret_val = fuse_main( 	(*((*(tea_ref.conf)).fuse_arguments)).argc,
									(*((*(tea_ref.conf)).fuse_arguments)).argv,
									(tea_ref.sys)->operations,
									NULL );

	// TODO add signals handler and call it when exit signal comes !!!

	return ret_val;
}
