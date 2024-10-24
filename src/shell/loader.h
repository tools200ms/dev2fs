/* loader.h --- Header file for loader.c.
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


#ifndef _DEV2FS_200MS_NET__SHELL_LOADER_H_
#define _DEV2FS_200MS_NET__SHELL_LOADER_H_

struct loader
{
	char *mnt_path, *str_path;

	int str_dir_path_length;

	uid_t str_uid, mnt_uid;
	gid_t str_gid, mnt_gid;
};
//engine_data;

/**
	validates cunfiguration and loads it to engine's structure
*/
void load_init( struct config *conf, struct loader *load );

/**
	releases engine's structure
*/
void load_release();// struct loader *load );

/**
	print information about current configuration
*/
void loadedconf_print_summary( FILE *stream, const char *exec_name, struct config_data *conf_data, struct loader *load );

#endif // _DEV2FS_200MS_NET__SHELL_LOADER_H_
