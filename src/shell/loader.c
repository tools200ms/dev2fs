/* loader.c --- Validates data read by config.c.
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

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <string.h>
#include <errno.h>


/* messages printing */
#include <stdio.h>
#include "../lib/messages.h"
/* ***************** */

#include "config.h"
#include "loader.h"

#include "../core/engine.h"

// include 'major' function
#include <sys/sysmacros.h>


void loadDirectoryTest( char *dir_name, char *purpose, struct stat *dir_statbuf );

//void load_operations( struct fuse_operations *operations );


void load_init( const struct config *conf, struct loader *load )
{
	//memset( &engi_data, 0, sizeof (struct engine_data) );

	struct stat str_dir_statbuf, mnt_dir_statbuf;
	//unsigned int source_dir_fd;

	if( lstat( conf->data.mount_point_path, &mnt_dir_statbuf ) != 0 )
	{
		MSG_DEBUG( "errno source", "%d", errno );

		perror( msg_getProgramName() );
		MSG_ERROR_AND_EXIT( "Failed to open mount point, resource might be mounted already" );
	}

	if( !S_ISDIR( mnt_dir_statbuf.st_mode ) )
	{
		MSG_ERROR_AND_EXIT( "Mount point must be a directory" );
	}

	// multiple storages
	if( lstat( conf->data.source_dir_path, &str_dir_statbuf ) != 0 )
	{
		MSG_DEBUG( "errno source", "%d", errno );

		perror( msg_getProgramName() );
		MSG_ERROR_AND_EXIT( "Failed to open " );
	}

	if( !S_ISDIR( str_dir_statbuf.st_mode ) )
	{
		MSG_ERROR_AND_EXIT( "File must be a directory" );
	}


	MSG_DEBUG( "source :", "%s", conf->data.source_dir_path );
	
	MSG_DEBUG( "DEV major", "%d", major( str_dir_statbuf.st_dev ) );
	MSG_DEBUG( "DEV major", "%d", major( mnt_dir_statbuf.st_dev ) );
	MSG_DEBUG( "ino", "%d", sizeof( str_dir_statbuf.st_ino ) );
	MSG_DEBUG( "ino", "%d", sizeof( long ) );

	/**
		check if source directory and mount point directory are not the same directory
	*/
	if( 	str_dir_statbuf.st_ino ==
			mnt_dir_statbuf.st_ino )
	{
		if( 	major( str_dir_statbuf.st_dev ) ==
				major( mnt_dir_statbuf.st_dev ) &&
				minor( str_dir_statbuf.st_dev ) ==
				minor( mnt_dir_statbuf.st_dev ) 	)
			{
				MSG_ERROR_AND_EXIT( "source directory and mount point are the same!" );
			}
	}


	/**
		copy source dir path to loader's structure, next the unnesesary slash from 
		the end of path will be erased
	*/

	load->str_uid = str_dir_statbuf.st_uid;
	load->str_gid = str_dir_statbuf.st_gid;

	load->mnt_uid = mnt_dir_statbuf.st_uid;
	load->mnt_gid = mnt_dir_statbuf.st_gid;


	char *source_dir_path = load->source_dir_path = strdup( conf->data.source_dir_path );
	/**
		cut slash symbols from the end of source directory path
	*/

	char *hash_loc = strrchr(source_dir_path, '/');

	if( hash_loc[1] == '\0' ) {
		hash_loc[0] = '\0';
	}

	MSG_DEBUG_BR;
}

void load_release()// struct loader *load )
{
	//if( load->source_dir_path != NULL )
	//	free( load->source_dir_path );
}


void loadedconf_print_summary(FILE* stream, struct config_data* conf_data, struct loader* load)
{
	fprintf	( stream, 	"%s started with the following parameters:\n"\
								"     storage dir (fixed uid/gid: %d/%d): %s\n"\
								"       mount dir (fixed uid/gid: %d/%d): %s\n"\
								" not allowed dir: %s\n"\
								"not allowed file: %s\n",
							msg_getProgramName(),
				load->str_uid, load->str_gid,
							( 	conf_data->source_dir_path != NULL ) ?
								conf_data->source_dir_path : MSG_NOTDEFINED,
				load->mnt_uid, load->mnt_gid,
							( 	conf_data->mount_point_path != NULL ) ?
								conf_data->mount_point_path : MSG_NOTDEFINED,
							( 	conf_data->notallowed_dir_name != NULL ) ?
								conf_data->notallowed_dir_name : MSG_NOTDEFINED,
							( 	conf_data->notallowed_file_name != NULL ) ?
								conf_data->notallowed_file_name : MSG_NOTDEFINED
				);

}

