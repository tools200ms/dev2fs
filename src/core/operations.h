/* operations.h --- Header file for operations.c.
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


#ifndef _DEV2FS_200MS_NET__CORE_OPERATIONS_H_
#define _DEV2FS_200MS_NET__CORE_OPERATIONS_H_


void operations_init( const struct loader *load, StringBuffer *str_buff );

/* teaFS initialize functions, defined in teafs.c */
void *dev2fs_init( struct fuse_conn_info *conn );

/* file system statistics */
int d2op_statfs( const char *path, struct statvfs *buf );

/* directories read operations */

int d2op_opendir( 	const char 				*path,
					struct fuse_file_info 	*fi 		);

int d2op_readdir(	const char 				*path,
					void 					*buf,
					fuse_fill_dir_t 		filler,
					off_t 					offset,
					struct fuse_file_info 	*fi		);

int d2op_releasedir( 	const char 				*path,
						struct fuse_file_info 	*fi );

/* directories modify operations */

int d2op_mkdir( 		const char 				*path,
						mode_t 					 mode );

int d2op_rmdir(		const char	*path );

/* files read operations */

int d2op_open(		const char 				*path,
					struct fuse_file_info 	*fi		);

int d2op_read(		const char 				*path,
					char 					*buf,
					size_t 					size,
					off_t 					offset,
					struct fuse_file_info 	*fi		);

int d2op_release( 	const char 				*path,
					struct fuse_file_info 	*fi	);

/* files modify operations */

int d2op_create( 	const char 					*path, 
					mode_t						 mode,
					struct fuse_file_info 	*fi );

int d2op_write( 		const char 					*path, 
							const char 					*buf, 
							size_t 						 size, 
							off_t 						 offset, 
							struct fuse_file_info	*fi 		);

int d2op_truncate( 	const char 				*path, 
							off_t						 length 	);

int d2op_flush( 		const char 					*path, 
							struct fuse_file_info	*fi 		);

int d2op_unlink(		const char *path 						);

/* files and directories modify operations */
int d2op_rename( 	const char 					*src_path, 
							const char 					*dest_path 	);

int d2op_chmod(		const char 					*path, 
							mode_t						 mode		);

/* attributes read operations */

int d2op_getattr(	const char 					*path, 
							struct stat 				*stbuf	);

int d2op_fgetattr(	const char 					*path, 
							struct stat 				*stbuf, 
							struct fuse_file_info	*fi	);

/* attributes modify operations */


/* links read operations */


/* links modify operations */

#endif // _DEV2FS_200MS_NET__CORE_OPERATIONS_H_
