/* massages.h --- Definitions and function declarations for message printing.
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


#ifndef _DEV2FS_200MS_NET__LIB_MESSAGES_H_
#define _DEV2FS_200MS_NET__LIB_MESSAGES_H_

#include <stdio.h>
#include <stdbool.h>

#include "messages_en.h"

#include "debug.h"

/**
	definitions of debug macros, it can be enabled with -DDEBUG gcc option or 
	make DEBUG=y
 */
#ifdef DEBUG

#define MSG_DEBUG( desc ) \
			fprintf( stdout, __FILE__ ": %d, %s\n", __LINE__, desc );

#define MSG_DEBUG_( desc, format, val ) \
			fprintf( stdout, __FILE__ ": %d, " desc " is '" format "'\n", __LINE__, val );

#define MSG_DEBUG_DEC( desc, val ) \
			fprintf( stdout, __FILE__ ": %d, " desc " is '%d'\n", __LINE__, val );

#define MSG_DEBUG_STR( desc, val ) \
			fprintf( stdout, __FILE__ ": %d, " desc " is '%s'\n", __LINE__, val );

#define MSG_DEBUG_BR \
			fprintf( stdout, "==============================================\n" );

#define MSG_DEBUG_PRINT_OFLAGS( oflag ) \
			{fprintf( stdout, "requested oflags:" ); \
			 if( oflag & O_RDONLY )		fprintf( stdout, " O_RDONLY" ); \
			 if( oflag & O_WRONLY )		fprintf( stdout, " O_WRONLY" ); \
			 if( oflag & O_RDWR )		fprintf( stdout, " O_RDWR" ); \
			 if( oflag & O_APPEND )		fprintf( stdout, " O_APPEND" ); \
			 if( oflag & O_CREAT )		fprintf( stdout, " O_CREAT" ); \
			 if( oflag & O_DSYNC )		fprintf( stdout, " O_DSYNC" ); \
			 if( oflag & O_EXCL )		fprintf( stdout, " O_EXCL" ); \
			 if( oflag & O_NOCTTY )		fprintf( stdout, " O_NOCTTY" ); \
			 if( oflag & O_NONBLOCK )	fprintf( stdout, " O_NONBLOCK" ); \
			 if( oflag & O_RSYNC )		fprintf( stdout, " O_RSYNC" ); \
			 if( oflag & O_SYNC )		fprintf( stdout, " O_SYNC" ); \
			 if( oflag & O_TRUNC )		fprintf( stdout, " O_TRUNC" ); \
			 fprintf( stdout, "\n" );}

#define MSG_DEBUG_PRINT_MODE_T( mode ) \
			{fprintf( stdout, "requested mode:" ); \
			 if( mode & S_IRWXU ) fprintf( stdout, " S_IRWXU (00700)" ); \
			 if( mode & S_IRUSR ) fprintf( stdout, " S_IRUSR (00400)" ); \
			 if( mode & S_IWUSR ) fprintf( stdout, " S_IWUSR (00200)" ); \
			 if( mode & S_IXUSR )	fprintf( stdout, " S_IXUSR (00100)" ); \
			 if( mode & S_IRWXG )	fprintf( stdout, " S_IRWXG (00070)" ); \
			 if( mode & S_IRGRP )	fprintf( stdout, " S_IRGRP (00040)" ); \
			 if( mode & S_IWGRP )	fprintf( stdout, " S_IWGRP (00020)" ); \
			 if( mode & S_IXGRP )	fprintf( stdout, " S_IXGRP (00010)" ); \
			 if( mode & S_IRWXO )	fprintf( stdout, " S_IRWXO (00007)" ); \
			 if( mode & S_IROTH )	fprintf( stdout, " S_IROTH (00004)" ); \
			 if( mode & S_IWOTH )	fprintf( stdout, " S_IWOTH (00002)" ); \
			 if( mode & S_IXOTH )	fprintf( stdout, " S_IXOTH (00001)" ); \
			 fprintf( stdout, "\n" );}

#else

/**
	just put nothing when DEBUG is not defined
*/
#define MSG_DEBUG( desc )
#define MSG_DEBUG_( desc, format, val )
#define MSG_DEBUG_DEC( desc, val )
#define MSG_DEBUG_STR( desc, val )
#define MSG_DEBUG_BR

#define MSG_DEBUG_PRINT_OFLAGS( oflag )
#define MSG_DEBUG_PRINT_MODE_T( mode )

#endif //DEBUG

#define PRINT_DEV2FS_VERSION_INFO( stream ) \
	fprintf(stream, "%s: %s\n", MSG_VERSION_INFO, DEV2FS_VERSION);

#define PRINT_DEV2FS_HELP( stream, exec_name ) \
	fprintf( stream, 	MSG_OPTIONS_HEADER, \
							exec_name, MSG_PROGRAM_USAGE, \
							MSG_GENERAL_OPTIONS, \
							MSG_DEV2FS_OPTIONS 	);

#define PRINT_WRONG_SYNTAX_ERROR( stream, exec_name ) \
	fprintf( stream, MSG_WRONG_SYNTAX, exec_name );


/**
	deffinitions of verbose mode functions and macros
*/
//extern int verbose_messages;
void msgSetVerboseMode();
bool msgIsVerboseModeSet();

#define MSG_VERBOSE( msg ) \
			if( msgIsVerboseModeSet() ) fprintf( stderr, msg "\n" );

#define MSG_VERBOSE_ARGS( format, msgs... ) \
			if( msgIsVerboseModeSet() ) fprintf( stderr, format "\n", msgs );

#define MSG_VERBOSE_FUN( print_fun ) \
			if( msgIsVerboseModeSet() ) print_fun( stderr );

#define MSG_VERBOSE_FUN_ARGS( print_fun, args... ) \
			if( msgIsVerboseModeSet() ) print_fun( stderr, args );

#define MSG_ERROR_AND_EXIT( msg ) \
			{ fprintf( stderr, "%s: %s\n", "dev2fs error", msg ); exit(1); }

#endif // _DEV2FS_200MS_NET__LIB_MESSAGES_H_
