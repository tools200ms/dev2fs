/* messages_en.h --- Definitions of English language messages, defined 
                     messages are displayed by program in the case of 
                     help request or error/warning occurrence.
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


/**
	definitions of message strings, all messages in this file should be in English, 
	in the case of translation appropriate file should be created, e.g. messages_pl.h
	containing translated strings.
*/

/**
	configure messages: 
*/

#define MSG_PROGRAM_USAGE 		"\"storage directory\" \"mount point\" [options]"

#define MSG_VERSION_INFO 		"teaFS filesystem version"


#define MSG_OPTIONS_HEADER		"usage: %s %s\n%s\n%s\n"

#define MSG_GENERAL_OPTIONS 	"general options:\n"\
										"    -o opt,[opt...]        mount options\n"\
										"    -h   --help            print help\n"\
										"    -V   --version         print version\n"

#define MSG_NOTTREE_OPTIONS	"teaFS options:\n"\
										"    -D NAME                equivalent to '-o notallowdir=NAME'\n"\
										"    -F NAME                equivalent to '-o notallowfile=NAME'\n"\
										"\n"\
										"    -o notallowdir=NAME    deny creation of directory with given NAME\n"\
										"    -o notallowfile=NAME   deny creation of file with given NAME\n"\
										"the quantifiers *(select zero or many) and ?(select none or one) can be used\n"\
										"in NAME to extend files/directories selection\n"

#define MSG_WRONG_SYNTAX 		"Wrong syntax, issue %s -h for help\n"

#define MSG_NOTDEFINED			""

/**
	mapper messages:
*/

#define MSG_TAGS_INFO			"Following tags have been found:\n"

#define MSG_RESMAP_INFO			"Following resources has been mapped:\n"

/**
	
*/


