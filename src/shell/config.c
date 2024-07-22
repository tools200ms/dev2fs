/* config.c --- Performs command-line arguments parsing.
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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* FUSE system */
#include "../dev2fs.h"
#include <fuse.h>
#include <fuse/fuse_opt.h>
/* *********** */

#include "config.h"


static int dev2fs_options_proc( 		void *data,
										const char *arg,
										int key,
										struct fuse_args *outargs 	);

#define DEV2FS_OPT_KEY( opt_template, opt_var, value ) \
			{ opt_template, offsetof( struct config_data, opt_var ), value }

enum	{ KEY_VERBOSE, KEY_VERSION, KEY_HELP };


static struct fuse_opt commandline_opts[] =
{
	DEV2FS_OPT_KEY( 	CONF_OPT_NOTALLOWED_DIR_NAME,
											notallowed_dir_name, 
											0 ), 
	DEV2FS_OPT_KEY( 	CONF_OPT_NOTALLOWED_FILE_NAME,
											notallowed_file_name, 
											0 ), 

	DEV2FS_OPT_KEY( 	CONF_ARG_NOTALLOWED_DIR_NAME,
											notallowed_dir_name, 
											0 ), 
	DEV2FS_OPT_KEY( 	CONF_ARG_NOTALLOWED_FILE_NAME,
											notallowed_file_name, 
											0 ), 

    // TODO: add verbose flag
	FUSE_OPT_KEY( CONF_ARG_DEBUG,				KEY_VERBOSE ),
	FUSE_OPT_KEY( CONF_OPT_DEBUG,				KEY_VERBOSE ),
	FUSE_OPT_KEY( CONF_ARG_FOREGROUND,		KEY_VERBOSE ), 

	FUSE_OPT_KEY( CONF_ARG_SHORT_VERSION, 	KEY_VERSION ), 
	FUSE_OPT_KEY( CONF_ARG_VERSION, 		KEY_VERSION ),
	FUSE_OPT_KEY( CONF_ARG_SHORT_HELP, 		KEY_HELP ),
	FUSE_OPT_KEY( CONF_ARG_HELP, 			KEY_HELP ),
	FUSE_OPT_END
};


enum {	CONF_PARSER_WRONG_ARG, 
		CONF_PARSER_VERSION_PRINT,
		CONF_PARSER_HELP_PRINT		};
static int conf_parser_program_abort_cause;

/*
	checks if obligatory parameters has been set
*/
static int confPostValidationErrors( struct config_data *conf_data )
{
	return 	conf_data->str_dir  == NULL ||
			conf_data->mnt_dir == NULL;
}


Config *conf_init( int argc, char *argv[] )
{
	Config *conf;

	if( (conf = malloc( sizeof (Config) )) == NULL )
		return NULL;

	memset( conf, 0, sizeof (Config) );

	if( (conf->fuse_arguments = malloc( sizeof (struct fuse_args) )) == NULL )
	{
		free( conf );
		return NULL;
	}

	/* initiates struct fuse_args
	 * does the same as FUSE_ARGS_INIT( argc, argv );*/

	conf->fuse_arguments->argc = argc;
	conf->fuse_arguments->argv = argv;
	conf->fuse_arguments->allocated = 0; 


	/* Sets default value for the case when dev2fs_options_proc() exits
		with an error value. */
	conf_parser_program_abort_cause = CONF_PARSER_WRONG_ARG;

	if( (fuse_opt_parse( 	conf->fuse_arguments, 
							&(conf->data),
							commandline_opts,
							dev2fs_options_proc ) 		== -1)
				|| confPostValidationErrors( &(conf->data) ) ) {
		FILE *stream = stderr;
		/* arguments for help/version print fuse_main() call */
		char *targv[2] = {NULL, NULL};

		switch( conf_parser_program_abort_cause )
		{
			/* display help message as it was requested (-h or --help was given as an argument) */
			case CONF_PARSER_HELP_PRINT:
				/* print's help message to stdout */
				PRINT_DEV2FS_HELP( stream = stdout, argv[0] );
				targv[0] = argv[0]; targv[1] = "-ho";
			break;
			/* prints help message in response to wrong/missing arguments */
			case CONF_PARSER_WRONG_ARG:
				PRINT_WRONG_SYNTAX_ERROR( stream, argv[0] );
			break;

			/* prints version as it was requested (by -V argument) */
			case CONF_PARSER_VERSION_PRINT:
				PRINT_DEV2FS_VERSION_INFO( stream = stdout );
				targv[0] = argv[0]; targv[1] = CONF_ARG_SHORT_VERSION;

			default:
				// TODO: add assertion
			break;
		}

		if( targv[0] != NULL )
		{
			/* By default fuse_main() prints help/version messages to stderr, 
				if stream is set to stdout, then value of stdout is assigned 
				to stderr, thus FUSE is forced to print to standard output. */
			/* If instead of 'glibc' 'musl libc' is used (case in Alpine),
			   stderr is defined as 'const', thus this assigment is not possible
			 */
			#if !defined( __MUSL__)
				stderr = stream;
			#endif

			/* prints help/version */
			fuse_main( 2, targv, NULL, NULL );
		}
		
		conf_destroy(conf);
		exit(stream != stdout);
	}

	return conf;
}

void conf_destroy( struct config *conf )
{
	if( conf->data.str_dir != NULL )
		free( conf->data.str_dir );

	if( conf->data.mnt_dir != NULL )
		free( conf->data.mnt_dir );

	if( conf->fuse_arguments != NULL )
		fuse_opt_free_args( conf->fuse_arguments );
	
	free(conf);
}

#define CONF_PARSE_KEEP_FLAG 	 return  1
#define CONF_PARSE_DISCARD_FLAG	 return  0
#define CONF_PARSE_SKIP_REMFLAGS return -1

static int dev2fs_options_proc( 	void *data,
									const char *arg,
									int key,
									struct fuse_args *outargs 	)
{
	struct config_data *conf_data_ptr = (struct config_data *)(data);

	MSG_DEBUG_DEC( "key", key );
	MSG_DEBUG_STR( "outargc", outargs->argv[0] );
	MSG_DEBUG_STR( "arg", arg );

	switch( key )
	{
		case FUSE_OPT_KEY_NONOPT:
		
			MSG_DEBUG( "FUSE_OPT_KEY_NONOPT" );
			MSG_DEBUG_BR;

			switch( 	((conf_data_ptr->str_dir == NULL) ? 0 : 1) +
						((conf_data_ptr->mnt_dir == NULL) ? 0 : 2) )
			{
				case 0:	// source_dir_path & mount_point_path are NULLs
					conf_data_ptr->str_dir = strdup( arg );
				CONF_PARSE_DISCARD_FLAG;
		
				case 1:	// only mount_point_path is NULL
					conf_data_ptr->mnt_dir = strdup( arg );
				CONF_PARSE_KEEP_FLAG;
		
				case 3: // source_dir_path & mount_point_path are set
				CONF_PARSE_SKIP_REMFLAGS;
			}

			//shouldn't happen
			//assert
			exit( 1 );

		// case FUSE_OPT_KEY_OPT


		case KEY_VERBOSE:
			msgSetVerboseMode();
			MSG_DEBUG( "FUSE_VERBOSE" );
			MSG_DEBUG_BR;

		//CONF_PARSE_DISCARD_FLAG;
		CONF_PARSE_KEEP_FLAG;
		
		case KEY_VERSION:

			MSG_DEBUG( "FUSE_VERSION" );
			MSG_DEBUG_BR;

			conf_parser_program_abort_cause = CONF_PARSER_VERSION_PRINT;
		CONF_PARSE_SKIP_REMFLAGS;

		case KEY_HELP:

			MSG_DEBUG( "FUSE_HELP" );
			MSG_DEBUG_BR;
		
			conf_parser_program_abort_cause = CONF_PARSER_HELP_PRINT;
		CONF_PARSE_SKIP_REMFLAGS;
		
		default:
			//check if fuse options are mached
		break;
	}

	// MSG_DEBUG_STR( "Keep flag for: ", arg );
	CONF_PARSE_KEEP_FLAG;
}
