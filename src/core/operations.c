/* operations.c --- Functions to handle teaFS I/O calls.
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

#include <sys/time.h>
#include <string.h>

#include "../shell/mapper.h"

#include "resources.h"

#include "engine.h"
#include "operations.h"


static StringBuffer *op_str_buff;
const struct loader *op_load;

uid_t buf_str_uid, buf_mnt_uid;
gid_t buf_str_gid, buf_mnt_gid;

void operations_init( const struct loader *load, StringBuffer *str_buff )
{
	op_load = load;
	buf_str_uid = load->str_uid;
	buf_mnt_uid = load->mnt_uid;
	buf_str_gid = load->str_gid;
	buf_mnt_gid = load->mnt_gid;

	op_str_buff = str_buff;
}


/* Dev2FS operations */

/* File system statistics,
	function is forwarding fs call.
 */
int d2op_statfs( const char *path, struct statvfs *buf )
{
	MSG_DEBUG( 		"=====> statfs operation called" );
	MSG_DEBUG_STR( 	"      path", path );

	char *full_path = strbuff_setFullPath( op_str_buff, path );

	//updatePath( path );
	int ret_val;

	if( (ret_val = statvfs( full_path, buf )) == -1 )
		perror( msg_getProgramName() );


	//relesePath( full_path );

	MSG_DEBUG( 	"-----> end of statfs" );
	return ret_val;
}

/* Directory open operation,
	function is forwarding fs call, fi->fh is set
	for readdir and releasedir operations.
 */
int d2op_opendir( const char 					*path,
							struct fuse_file_info 	*fi 		)
{
	MSG_DEBUG( 		"=====> opendir operation called" );
	MSG_DEBUG_STR( 	"      path", path );

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

	MSG_DEBUG( 	"-----> end of opendir" );
	return 0;
}

/* Directory readdir operation,
	function is forwarding fs call, handler from fi->fh is used to
	read directory.
 */
int d2op_readdir(	const char 					*path,
							void 					*buf,
							fuse_fill_dir_t 		filler,
							off_t 					offset,
							struct fuse_file_info 	*fi	)
{
	MSG_DEBUG( 		"=====> readdir operation called" );
	MSG_DEBUG_STR( 	"      path", path );

	struct dir_handler *d_handler;
	struct dirent *dir_entry;

	memcpy( &d_handler, &(fi->fh), sizeof ( struct dir_handler * ) );

	while( (dir_entry = readdir( d_handler->dir_ptr )) != NULL )
	{
		filler( buf, dir_entry->d_name, NULL, 0 );
	}

	MSG_DEBUG( 	"-----> end of readdir" );

	return 0;
}

/* Directory releasedir operation,
	releases fi->fh handler.
 */
int d2op_releasedir( 	const char 					*path,
						struct fuse_file_info 	*fi 		)
{
	MSG_DEBUG( 		"=====> releasedir operation called" );
	MSG_DEBUG_STR( 	"      path", path );

	struct dir_handler *d_handler;

	memcpy( &d_handler, &(fi->fh), sizeof ( struct dir_handler * ) );

	closedir( d_handler->dir_ptr );

	free( d_handler );

	MSG_DEBUG( 	"-----> end of releasedir" );

	return 0;
}

/* Directories modify operations */

/* Directory mkdir operation,
	creates directories in multiple locations if needed.
 */
int d2op_mkdir( 	const char 					*path,
						mode_t 					mode 	)
{
	MSG_DEBUG( 	"=====> mkdir operation called" );
	MSG_DEBUG_STR( 			"      path", path );

	int ret_val;
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	ret_val = mkdir( full_path, mode );
	chown( full_path, buf_str_uid, buf_str_gid );

	return ret_val;
}

int d2op_rmdir( const char *path )
{
	MSG_DEBUG(		"=====> rmdir operation called" );
	MSG_DEBUG_STR( 	"      path", path );


	int ret_val;
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	if( (ret_val = rmdir( full_path )) == -1 ) {
			perror( msg_getProgramName() );
	}

	MSG_DEBUG( 	"-----> end of rmdir" );
	return ret_val;
}

/* files read operations */

int d2op_open(	const char 	*path,
						struct fuse_file_info 	*fi	)
{
	MSG_DEBUG( 	"=====> open operation called" );
	MSG_DEBUG_STR( 			"      path", path );

	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	int ret_val = 0;

	struct file_handler *f_handler = malloc( sizeof (struct file_handler) );
	//f_handler->fd = fd;

	MSG_DEBUG_( "fi->fh", "%lu", fi->fh );
	//fi->fh = 0;//(uint64_t)(f_handler);
	//fi->fh = (long)f_handler;
	memcpy( &(fi->fh), &f_handler, sizeof( struct file_handler * ) );

	if( (f_handler->fd = open( full_path, fi->flags )) == -1 )
	{
		perror( msg_getProgramName() );
		ret_val = -1;
	}


	//relesePath( full_path );

	MSG_DEBUG( 	"-----> end of open" );
	return ret_val;
}

int d2op_read(	const char 					*path,
						char 				*buf,
						size_t 						size,
						off_t 						offset,
						struct fuse_file_info 	*fi	)
{
	MSG_DEBUG( 		"=====> read operation called" );
	MSG_DEBUG_STR( 	"      path", path );


	MSG_DEBUG( 	"=====> read operation called" );
	size_t read_bytes;

	MSG_DEBUG_( "      offset", "%d", (int)offset );
	MSG_DEBUG_( "      size", "%d", (int)size );

	struct file_handler *f_handler;// = (struct file_handler *)(fi->fh);
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );
	MSG_DEBUG_( "fi->fh", "%lu", fi->fh );
	MSG_DEBUG_( "fd", "%d", f_handler->fd );

	int fd = f_handler->fd;

	if( (read_bytes = pread( fd, buf, size, offset )) == -1 )
	{
		perror( msg_getProgramName() );
		//return 0;
	}

	MSG_DEBUG_( "      read_bytes", "%lu", read_bytes );

	MSG_DEBUG( 	"-----> end of read" );
	return read_bytes;
}

int d2op_release( 	const char 					*path,
							struct fuse_file_info 	*fi	 )
{
	//char *full_path = updatePath( path );
	MSG_DEBUG( 		"=====> release operation called" );
	MSG_DEBUG_STR( 	"      path", path );
	//relesePath( full_path );

	struct file_handler *f_handler; // = (struct file_handler *)(fi->fh);
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );


	close( f_handler->fd );
	free( f_handler );

	MSG_DEBUG( 	"-----> end of release" );
	return 0; // ignored by FUSE
}

/* file modify operations */

int d2op_create( 	const char 					*path,
							mode_t						 mode,
							struct fuse_file_info 	*fi 		)
{
	MSG_DEBUG( 	"=====> create operation called" );
	MSG_DEBUG_STR( 		"      path", path );

	char *full_path = strbuff_setFullPath( op_str_buff, path );

	struct file_handler *f_handler = malloc( sizeof (struct file_handler) );

	if( (f_handler->fd = open( full_path, fi->flags, mode )) == -1 )
	{
		perror( msg_getProgramName() );
		free( f_handler );

		return -1;
	}

	chown( full_path, buf_str_uid, buf_str_gid );

	memcpy( &(fi->fh), &f_handler, sizeof( struct file_handler * ) );

	MSG_DEBUG( 	"-----> end of create" );
	return 0;
}

int d2op_write( 	const char 					*path,
						const char 					*buf,
						size_t 						 size,
						off_t 						 offset,
						struct fuse_file_info	*fi 		)
{
	MSG_DEBUG( 	"=====> write operation called" );
	MSG_DEBUG_STR( 		"      path", path );

	size_t write_bytes;
	struct file_handler *f_handler;
	(void) path;

	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );

	//int fd = f_handler->fd;

	MSG_DEBUG_( "      size", "%lu", size );
	MSG_DEBUG_( "      offset", "%lu", offset );
	MSG_DEBUG_( "    FD", "%d", f_handler->fd );

	if( (write_bytes = pwrite( f_handler->fd, buf, size, offset )) == -1 )
	{
		perror( msg_getProgramName() );
	}

	MSG_DEBUG_( 			"      write_bytes", "%lu", write_bytes );

	MSG_DEBUG( 	"-----> end of write" );
	return write_bytes;
}

int d2op_truncate( 	const char 				*path,
							off_t						 length 	)
{
	MSG_DEBUG( 	"=====> truncate operation called" );
	MSG_DEBUG_STR( 	"      path", path );

	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val;

	if( (ret_val = truncate( full_path, length )) == -1 )
	{
		perror( msg_getProgramName() );
	}

	//relesePath( full_path );

	MSG_DEBUG( 	"-----> end of truncate" );
	return ret_val;
}

int d2op_flush( 	const char 					*path,
						struct fuse_file_info	*fi 		)
{
	MSG_DEBUG( 		"=====> flush operation called" );
	MSG_DEBUG_STR( 	"      path", path );

	//struct file_handler *f_handler;
	//memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );

	//flush


	MSG_DEBUG( 	"-----> end of flush" );
	return 0;
}

int d2op_fsync(		const char 				*path,
					int 					 isdatasync,
					struct fuse_file_info 	*fi	) {
	MSG_DEBUG( 		"=====> fsync operation called" );
	MSG_DEBUG_STR( 	"      path", path );

	int ret_val;
	struct file_handler *f_handler;

	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );

	ret_val = (isdatasync != 0) ? fdatasync(f_handler->fd) :
									fsync(f_handler->fd);


	MSG_DEBUG( 	"-----> end of fsync" );
	return ret_val;
}

int d2op_fsyncdir(	const char 				*path,
					int 					 isdatasync,
					struct fuse_file_info 	*fi	) {
	MSG_DEBUG( 		"=====> fsyncdir operation called" );
	MSG_DEBUG_STR( 	"      path", path );

	int ret_val = 0;
	//struct dir_handler *d_handler;

	//memcpy( &d_handler, &(fi->fh), sizeof ( struct dir_handler * ) );

	/*ret_val = (isdatasync != 0) ? fdatasync(d_handler->fd) :
									fsync(d_handler->fd);


	MSG_DEBUG( 	"-----> end of fsync" );*/
	return ret_val;
}

int d2op_unlink( const char *path )
{
	MSG_DEBUG( 	"=====> unlink operation called" );
	MSG_DEBUG_STR( 	"      path", path );
	//char *full_path = updatePath( path );

	int ret_val;
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	if( (ret_val = unlink( full_path )) == -1 )
	{
		perror( msg_getProgramName() );
	}

	MSG_DEBUG( 	"-----> end of unlink" );
	return ret_val;

	//relesePath( full_path );
}

/* files and directories modify operations */
int d2op_rename( const char *src_path, const char *dest_path )
{
	MSG_DEBUG( 	"=====> rename operation called" );
	MSG_DEBUG_STR( 			"      src_path", src_path );
	MSG_DEBUG_STR( 			"      dest_path", dest_path );

	int ret_val;

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

	MSG_DEBUG( 	"-----> end of rename" );
	return ret_val;
}


/* attributes read operations */

int d2op_getattr(	const char 	*path,
							struct stat *stbuf	)
{
	MSG_DEBUG( 	"=====> getattr operation called" );

	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val = 0;
	MSG_DEBUG_STR( "   full path", full_path );
	//memset( stbuf, 0, sizeof(struct stat) );

	if( lstat( full_path, stbuf ) != 0 )
	{
		perror( msg_getProgramName() );
		ret_val = -ENOENT;
	}

	// overwrite uis and gid
	stbuf->st_uid = buf_mnt_uid;
	stbuf->st_gid = buf_mnt_gid;


	//relesePath( full_path );

	MSG_DEBUG( 	"-----> end of getattr" );
	return ret_val;
}

int d2op_fgetattr(	const char 			*path,
							struct stat *stbuf,
							struct fuse_file_info	*fi	)
{
	MSG_DEBUG( 	"=====> fgetattr operation called" );
	MSG_DEBUG_STR( 	"   path", path );

	int ret_val = 0;

	struct file_handler *f_handler;
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );

	if( fstat( f_handler->fd, stbuf ) != 0 )
	{
		perror( msg_getProgramName() );
		ret_val = -ENOENT;
	}

	// overwrite uis and gid
	stbuf->st_uid = buf_mnt_uid;
	stbuf->st_gid = buf_mnt_gid;

	MSG_DEBUG( 	"-----> end of fgetattr" );
	return ret_val;
}

int d2op_chmod( const char *path, mode_t mode )
{
	MSG_DEBUG( 	"=====> chmod operation called" );
	MSG_DEBUG_STR( 	"   path", path );

	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val;

	if( (ret_val = chmod( full_path, mode )) == -1 )
	{
		perror( msg_getProgramName() );
	}

	//relesePath( full_path );

	MSG_DEBUG( 	"-----> end of chmod" );
	return ret_val;
}

int d2op_chown( const char *path, uid_t uid, gid_t gid)
{
	MSG_DEBUG( 	"=====> chown operation called" );
	MSG_DEBUG_STR( 	"   path", path );

	int ret_val = 0;
	// TODO: check if fs is read only
	// if so: return EROFS;

	if( uid == buf_mnt_uid && gid == buf_mnt_gid )
	{
		// remap permissions:
		ret_val = chown(
			strbuff_setFullPath( op_str_buff, path ),
			buf_str_uid, buf_str_gid);
	} else {
		ret_val = EPERM;
	}

	// lack of permisions to change owner
	MSG_DEBUG_DEC( 	"-----> end of chown", ret_val );
	return ret_val;
}

int d2op_utimens( const char *path, const struct timespec tv_am[2] )
{
	MSG_DEBUG( 	"=====> utimens operation called" );
	MSG_DEBUG_STR( 	"   path", path );
	int ret_val;
	struct timeval tv_am_str[2];

	/* Copy access time and modification time,
	 * utimes takes a time at lower resolution
	 * (msec) vs. (usec) provided by FUSE
	 */
	// NOTE: we could round the divvision,
	// but we operate on microsecounds, what is
	// very precaise taking into acount that it's
	// 'just' file aacess/modification time
	// the better strategy seems to be to
	// truncate division resolts reather then
	// overhead with casting and calling
	// math. function

	// Pass access time:
	tv_am_str[0].tv_sec = tv_am[0].tv_sec;
	tv_am_str[0].tv_usec = (tv_am[0].tv_nsec / 1000);

	// Pass modification time:
	tv_am_str[1].tv_sec = tv_am[0].tv_sec;
	tv_am_str[1].tv_usec = (tv_am[0].tv_nsec / 1000);

	// TODO: in this case don't follow symb. links,
	// but shall be implemented
	ret_val = utimes(strbuff_setFullPath( op_str_buff, path ),
		tv_am_str
	);

	MSG_DEBUG( 	"-----> end of utimens" );
	return ret_val;
}

int d2op_access(const char *path, int mask) {
	MSG_DEBUG( 	"=====> access operation called" );
	MSG_DEBUG_STR( 	"   path", path );

	int ret_val;

	ret_val = access(strbuff_setFullPath( op_str_buff, path ), mask);

	MSG_DEBUG( 	"-----> end of access" );
	return ret_val;
}

/*************************************************************/
