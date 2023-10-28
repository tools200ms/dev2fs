/* operations.c --- Functions to handle teaFS I/O calls.
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


/* messages printing */
#include <stdio.h>
#include "../lib/messages.h"
/* ***************** */

/* FUSE system */
#include "../dev2fs.h"
#include <fuse.h>
/* *********** */

/* filesystem, files and directories operations */
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
/* ******************************************** */


#include <string.h>
#include "../shell/mapper.h"

#include "../lib/tags.h"
#include "resources.h"

#include "engine.h"
#include "operations.h"

#include "../format/hlmapping.h"


static StringBuffer *op_str_buff;



void operations_init( StringBuffer *str_buff )
{
	op_str_buff = str_buff;
}


/* TEAFS OPERATIONS */

/* File system statistics, 
	function is forwarding fs call.
 */
int teaop_statfs( const char *path, struct statvfs *buf )
{
	MSG_DEBUG_INFO( 	"=====> statfs operation called" );
	MSG_DEBUG( 			"      path", "%s", path );
	
	char *full_path = strbuff_setFullPath( op_str_buff, path );
	
	//updatePath( path );
	int ret_val;
	
	if( (ret_val = statvfs( full_path, buf )) == -1 )
		perror( msg_getProgramName() );
	

	//relesePath( full_path );
	
	MSG_DEBUG_INFO( 	"-----> end of statfs" );
	return ret_val;
}

/* Directory open operation, 
	function is forwarding fs call, fi->fh is set 
	for readdir and releasedir operations.
 */
int teaop_opendir( const char 					*path, 
							struct fuse_file_info 	*fi 		)
{
	MSG_DEBUG_INFO( 	"=====> opendir operation called" );
	MSG_DEBUG( 			"      path", "%s", path );
	
	/*******************************************************/
	/*******************************************************/
	
	struct dir_handler *d_handler = malloc( sizeof ( struct dir_handler ) );
	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	if( (d_handler->dir_ptr = opendir( full_path )) == NULL )
	{
		perror( msg_getProgramName() );
	}

	//relesePath( full_path );

	memcpy( &(fi->fh), &d_handler, sizeof( struct dir_handler * ) );
	
	MSG_DEBUG_INFO( 	"-----> end of opendir" );
	return 0;
}

/* Directory readdir operation, 
	function is forwarding fs call, handler from fi->fh is used to 
	read directory.
 */
int teaop_readdir(	const char 					*path, 
							void 					*buf,
							fuse_fill_dir_t 		filler,
							off_t 					offset,
							struct fuse_file_info 	*fi	)
{
	MSG_DEBUG_INFO( 	"=====> readdir operation called" );
	MSG_DEBUG( 			"      path", "%s", path );

	struct dir_handler *d_handler;
	struct dirent *dir_entry;

	memcpy( &d_handler, &(fi->fh), sizeof ( struct dir_handler * ) );

	while( (dir_entry = readdir( d_handler->dir_ptr )) != NULL )
	{
		filler( buf, dir_entry->d_name, NULL, 0 );
	}

	MSG_DEBUG_INFO( 	"-----> end of readdir" );

	return 0;
}

/* Directory releasedir operation, 
	releases fi->fh handler.
 */
int teaop_releasedir( 	const char 					*path, 
						struct fuse_file_info 	*fi 		)
{
	MSG_DEBUG_INFO( 	"=====> releasedir operation called" );
	MSG_DEBUG( 			"      path", "%s", path );

	struct dir_handler *d_handler;
	
	memcpy( &d_handler, &(fi->fh), sizeof ( struct dir_handler * ) );
	
	closedir( d_handler->dir_ptr );
	
	free( d_handler );
	
	MSG_DEBUG_INFO( 	"-----> end of releasedir" );
	
	return 0;
}

/* Directories modify operations */

/* Directory mkdir operation, 
	creates directories in multiple locations if needed.
 */
int teaop_mkdir( 	const char 					*path, 
						mode_t 						 mode 	)
{
	MSG_DEBUG_INFO( 	"=====> mkdir operation called" );
	MSG_DEBUG( 			"      path", "%s", path );

	int ret_val;
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	ret_val = mkdir( full_path, S_IRWXU | S_IRWXG );
	
	return ret_val;
}

int teaop_rmdir( const char *path )
{
	MSG_DEBUG_INFO( 	"=====> rmdir operation called" );
	MSG_DEBUG( 			"      path", "%s", path );


	int ret_val;
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	if( (ret_val = rmdir( full_path )) == -1 ) {
			perror( msg_getProgramName() );
	}

	MSG_DEBUG_INFO( 	"-----> end of rmdir" );
	return ret_val;
}

/* files read operations */

int teaop_open(	const char 					*path, 
						struct fuse_file_info 	*fi	)
{
	MSG_DEBUG_INFO( 	"=====> open operation called" );
	MSG_DEBUG( 			"      path", "%s", path );

	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	int ret_val = 0;

	struct file_handler *f_handler = malloc( sizeof (struct file_handler) );
	//f_handler->fd = fd;

	MSG_DEBUG( "fi->fh", "%lu", fi->fh );
	//fi->fh = 0;//(uint64_t)(f_handler);
	//fi->fh = (long)f_handler;
	memcpy( &(fi->fh), &f_handler, sizeof( struct file_handler * ) );

	if( (f_handler->fd = open( full_path, fi->flags )) == -1 )
	{
		perror( msg_getProgramName() );
		ret_val = -1;
	}


	//relesePath( full_path );

	MSG_DEBUG_INFO( 	"-----> end of open" );
	return ret_val;
}

int teaop_read(	const char 					*path, 
						char 							*buf, 
						size_t 						size, 
						off_t 						offset, 
						struct fuse_file_info 	*fi	)
{
	MSG_DEBUG_INFO( 	"=====> read operation called" );
	MSG_DEBUG( 			"      path", "%s", path );


	MSG_DEBUG_INFO( 	"=====> read operation called" );
	size_t read_bytes;

	MSG_DEBUG( 			"      offset", "%d", (int)offset );
	MSG_DEBUG( 			"      size", "%d", (int)size );

	struct file_handler *f_handler;// = (struct file_handler *)(fi->fh);
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );
	MSG_DEBUG( "fi->fh", "%lu", fi->fh );
	MSG_DEBUG( "fd", "%d", f_handler->fd );

	int fd = f_handler->fd;

	if( (read_bytes = pread( fd, buf, size, offset )) == -1 )
	{
		perror( msg_getProgramName() );
		//return 0;
	}

	MSG_DEBUG( 			"      read_bytes", "%d", read_bytes );

	MSG_DEBUG_INFO( 	"-----> end of read" );
	return read_bytes;
}

int teaop_release( 	const char 					*path, 
							struct fuse_file_info 	*fi	 )
{
	//char *full_path = updatePath( path );
	MSG_DEBUG_INFO( 	"=====> release operation called" );
	MSG_DEBUG( 			"      path", "%s", path );
	//relesePath( full_path );

	struct file_handler *f_handler; // = (struct file_handler *)(fi->fh);
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );


	close( f_handler->fd );
	free( f_handler );

	MSG_DEBUG_INFO( 	"-----> end of release" );
	return 0; // ignored by FUSE
}

/* file modify operations */

int teaop_create( 	const char 					*path, 
							mode_t						 mode, 
							struct fuse_file_info 	*fi 		)
{
	MSG_DEBUG_INFO( 	"=====> create operation called" );
	MSG_DEBUG( 			"      path", "%s", path );

	MapNode *map_node;
	char *file_name;

	char *full_path = strbuff_setFullPath( op_str_buff, path );
	
	struct file_handler *f_handler = malloc( sizeof (struct file_handler) );

	if( (f_handler->fd = open( full_path, fi->flags, mode )) == -1 )
	{
		perror( msg_getProgramName() );
		free( f_handler );

		return -1;
	}
	
	memcpy( &(fi->fh), &f_handler, sizeof( struct file_handler * ) );
	
	MSG_DEBUG_INFO( 	"-----> end of create" );
	return 0;
}

int teaop_write( 	const char 					*path, 
						const char 					*buf, 
						size_t 						 size, 
						off_t 						 offset, 
						struct fuse_file_info	*fi 		)
{
	MSG_DEBUG_INFO( 	"=====> write operation called" );
	MSG_DEBUG( 			"      path", "%s", path );

	size_t write_bytes;
	struct file_handler *f_handler;
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );

	//int fd = f_handler->fd;

	MSG_DEBUG( 			"      size", "%d", size );
	MSG_DEBUG( 			"      offset", "%d", offset );
	MSG_DEBUG( "    FD", "%d", f_handler->fd );

	if( (write_bytes = pwrite( f_handler->fd, buf, size, offset )) == -1 )
	{
		perror( msg_getProgramName() );
	}

	MSG_DEBUG( 			"      write_bytes", "%d", write_bytes );

	MSG_DEBUG_INFO( 	"-----> end of write" );
	return write_bytes;
}

int teaop_truncate( 	const char 				*path, 
							off_t						 length 	)
{
	MSG_DEBUG_INFO( 	"=====> truncate operation called" );
	MSG_DEBUG( 			"      path", "%s", path );
	
	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val;
	
	if( (ret_val = truncate( full_path, length )) == -1 )
	{
		perror( msg_getProgramName() );
	}

	//relesePath( full_path );
	
	MSG_DEBUG_INFO( 	"-----> end of truncate" );
	return ret_val;
}

int teaop_flush( 	const char 					*path, 
						struct fuse_file_info	*fi 		)
{
	MSG_DEBUG_INFO( 	"=====> flush operation called" );
	MSG_DEBUG( 			"      path", "%s", path );

	//struct file_handler *f_handler;
	//memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );



	MSG_DEBUG_INFO( 	"-----> end of flush" );
	return 0;
}

int teaop_unlink( const char *path )
{
	MSG_DEBUG_INFO( 	"=====> unlink operation called" );
	MSG_DEBUG( 			"      path", "%s", path );
	//char *full_path = updatePath( path );

	int ret_val;
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	if( (ret_val = unlink( full_path )) == -1 )
	{
		perror( msg_getProgramName() );
	}

	MSG_DEBUG_INFO( 	"-----> end of unlink" );
	return ret_val;

	//relesePath( full_path );
}

/* files and directories modify operations */
int teaop_rename( const char *src_path, const char *dest_path )
{
	MSG_DEBUG_INFO( 	"=====> rename operation called" );
	MSG_DEBUG( 			"      src_path", "%s", src_path );
	MSG_DEBUG( 			"      dest_path", "%s", dest_path );
	
	int ret_val;
	size_t path_len;

	char *_src_full_path, *src_full_path, *dest_full_path;

	_src_full_path = strbuff_setFullPath( op_str_buff, src_path );

	src_full_path = malloc( (sizeof (char)) * (strlen(_src_full_path) + 1) );

	strcpy(src_full_path, _src_full_path);

	dest_full_path = strbuff_setFullPath( op_str_buff, dest_path );

	if( (ret_val = rename( src_full_path, dest_full_path )) == -1 )
	{
		perror( msg_getProgramName() );
	}

	free(src_full_path);

	MSG_DEBUG_INFO( 	"-----> end of rename" );
	return ret_val;
}

int teaop_chmod( const char *path, mode_t mode )
{
	MSG_DEBUG_INFO( 	"=====> chmod operation called" );
	MSG_DEBUG( 			"   path", "%s", path );

	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val;

	if( (ret_val = chmod( full_path, mode )) == -1 )
	{
		perror( msg_getProgramName() );
	}

	//relesePath( full_path );

	MSG_DEBUG_INFO( 	"-----> end of chmod" );
	return ret_val;
}

/* attributes read operations */

int teaop_getattr(	const char 	*path, 
							struct stat *stbuf	)
{
	MSG_DEBUG_INFO( 	"=====> getattr operation called" );
	
	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val = 0;
	MSG_DEBUG( 			"   full_path", "%s", full_path );
	//memset( stbuf, 0, sizeof(struct stat) );

	if( lstat( full_path, stbuf ) != 0 )
	{
		perror( msg_getProgramName() );
		ret_val = -ENOENT;
	}

	//relesePath( full_path );

	MSG_DEBUG_INFO( 	"-----> end of getattr" );
	return ret_val;
}

int teaop_fgetattr(	const char 					*path, 
							struct stat 				*stbuf, 
							struct fuse_file_info	*fi	)
{
	MSG_DEBUG_INFO( 	"=====> fgetattr operation called" );
	MSG_DEBUG( 			"   path", "%s", path );

	int ret_val = 0;

	struct file_handler *f_handler;
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );
	
	if( fstat( f_handler->fd, stbuf ) != 0 )
	{
		perror( msg_getProgramName() );
		ret_val = -ENOENT;
	}
	
	MSG_DEBUG_INFO( 	"-----> end of fgetattr" );
	return ret_val;
}

/* attributes modify operations */


/* links read operations */



/* links modify operations */

/*************************************************************/
