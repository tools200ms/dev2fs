/* messages.c --- Functions for message printing.
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

#include "../devfs.h"
#include "messages.h"

static char *msg_program_name = NULL;

void msg_setProgramName( char *program_name )
{
	msg_program_name = program_name;
}

char *msg_getProgramName()
{
	return msg_program_name;
}

/**
	formatting functions
*/

void msg_notTreeVersion( FILE *stream )
{
	fprintf( stream, "%s: %s\n", MSG_VERSION_INFO, DEV2FS_VERSION );
}

void msg_notTreeHelp( FILE *stream )
{
	fprintf( stream, 	MSG_OPTIONS_HEADER, 
							msg_program_name, MSG_PROGRAM_USAGE, 
							MSG_GENERAL_OPTIONS, 
							MSG_NOTTREE_OPTIONS 	);
}

void msg_notTreeWrongSyntax( FILE *stream )
{
	fprintf( stream, MSG_WRONG_SYNTAX, msg_program_name );
}

/**
	
*/
int verbose_messages = 0;
void msgSetVerboseMode()	{ verbose_messages = 1; }

