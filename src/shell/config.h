/* config.h --- Header file for config.c.
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


#ifndef _DEV2FS_200MS_NET__SHELL_CONFIG_H_
#define _DEV2FS_200MS_NET__SHELL_CONFIG_H_

#include "../lib/messages.h"

/* general flags used by dev2fs */
/* forces dev2fs to print it's and FUSE version/help and exit
*/
#define CONF_ARG_VERSION				"--version"
#define CONF_ARG_SHORT_VERSION			"-V"
#define CONF_ARG_HELP					"--help"
#define CONF_ARG_SHORT_HELP				"-h"

/* debug and foreground passed as argument or option
	forces dev2fs to activate verbose mode
*/
#define CONF_OPT_DEBUG					"debug"
#define CONF_ARG_DEBUG					"-d"
#define CONF_ARG_FOREGROUND				"-f"


/* nottree flags definitions */
#define CONF_OPT_NOTALLOWED_DIR_NAME	"notallowdir=%s"
#define CONF_OPT_NOTALLOWED_FILE_NAME	"notallowfile=%s"

#define CONF_ARG_MAP					"-m %s"
#define CONF_ARG_NOTALLOWED_DIR_NAME	"-D %s"
#define CONF_ARG_NOTALLOWED_FILE_NAME	"-F %s"


typedef struct config_data
{
	char 	*mnt_dir, // mountpoint path
			*str_dir; // storage direcotory path

	/** notTree options **/
	char 	*notallowed_dir_name, 
			*notallowed_file_name;
} ConfigData;

typedef struct config
{
	ConfigData data;

	/** FUSE parameters **/
	struct fuse_args *fuse_arguments;
} Config;


//char *CONF_PROGRAM_NAME;


Config *conf_init( int argc, char *argv[] );
void conf_destroy( Config *conf );

#endif // _DEV2FS_200MS_NET__SHELL_CONFIG_H_
