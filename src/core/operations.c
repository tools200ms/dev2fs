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

#include "../lib/op_stats.h"

#include "engine.h"
#include "operations.h"


static StringBuffer *op_str_buff;
const struct loader *op_load;
Stats *op_stat;

uid_t buf_str_uid, buf_mnt_uid;
gid_t buf_str_gid, buf_mnt_gid;

void operations_init( const struct loader *load, StringBuffer *str_buff )
{
	op_load = load;
	op_stat = stat_init();

	buf_str_uid = load->str_uid;
	buf_mnt_uid = load->mnt_uid;
	buf_str_gid = load->str_gid;
	buf_mnt_gid = load->mnt_gid;

	op_str_buff = str_buff;
}

void operations_destroy() {
	if(op_stat != NULL) {
		stat_destroy(op_stat);
	}
}

/* Dev2FS operations */

/* File system statistics,
	function is forwarding fs call.
 */
int d2op_statfs(const char *path, struct statvfs *buf)
{
	MSG_OPSTAT_VERBOSE("STATFS", path);

	char *full_path = strbuff_setFullPath( op_str_buff, path );

	//updatePath( path );
	int ret_val;

	ret_val = statvfs( full_path, buf );

	strbuff_release( full_path );

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

/* Directory open operation,
	function is forwarding fs call, fi->fh is set
	for readdir and releasedir operations.
 */
int d2op_opendir(const char *path,
				struct fuse_file_info 	*fi)
{
	MSG_OPSTAT_VERBOSE("OPENDIR", path);

	struct dir_handler *d_handler = malloc( sizeof ( struct dir_handler ) );
	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val = 0;

	if((d_handler->dir_ptr = opendir(full_path)) != NULL) {
		memcpy( &(fi->fh), &d_handler, sizeof( struct dir_handler * ) );
	}
	else {
		ret_val = -1;
	}

	//relesePath( full_path );

	MSG_OPSTAT_SUMMARY();
	return ret_val;
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
	MSG_OPSTAT_VERBOSE("READDIR", path);

	struct dir_handler *d_handler;
	struct dirent *dir_entry;

	memcpy( &d_handler, &(fi->fh), sizeof ( struct dir_handler * ) );

	while( (dir_entry = readdir( d_handler->dir_ptr )) != NULL )
	{
		filler( buf, dir_entry->d_name, NULL, 0 );
	}

	MSG_OPSTAT_SUMMARY();
	return 0;
}

/* Directory releasedir operation,
	releases fi->fh handler.
 */
int d2op_releasedir( 	const char 					*path,
						struct fuse_file_info 	*fi 		)
{
	MSG_OPSTAT_VERBOSE("RELEASEDIR", path);

	struct dir_handler *d_handler;

	memcpy( &d_handler, &(fi->fh), sizeof ( struct dir_handler * ) );

	closedir( d_handler->dir_ptr );

	free( d_handler );

	MSG_OPSTAT_SUMMARY();
	return 0;
}

/* Directories modify operations */

/* Directory mkdir operation,
	creates directories in multiple locations if needed.
 */
int d2op_mkdir( 	const char 					*path,
						mode_t 					mode 	)
{
	MSG_OPSTAT_VERBOSE("MKDIR", path );

	int ret_val;
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	ret_val = mkdir( full_path, mode );
	chown( full_path, buf_str_uid, buf_str_gid );

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_rmdir( const char *path )
{
	MSG_OPSTAT_VERBOSE("RMDIR", path );

	int ret_val;
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	ret_val = rmdir( full_path );

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

/* files read operations */

int d2op_open(	const char 	*path,
						struct fuse_file_info 	*fi	)
{
	MSG_OPSTAT_VERBOSE("OPEN", path );

	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	int ret_val = 0;

	struct file_handler *f_handler = malloc( sizeof (struct file_handler) );
	//f_handler->fd = fd;

	MSG_DEBUG_( "fi->fh", "%lu", fi->fh );
	//fi->fh = 0;//(uint64_t)(f_handler);
	//fi->fh = (long)f_handler;
	memcpy( &(fi->fh), &f_handler, sizeof( struct file_handler * ) );

	if( (f_handler->fd = open( full_path, fi->flags )) == -1 ) {
		ret_val = -1;
	}


	//relesePath( full_path );

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_read(	const char 					*path,
						char 				*buf,
						size_t 						size,
						off_t 						offset,
						struct fuse_file_info 	*fi	)
{
	MSG_OPSTAT_VERBOSE("READ", path );

	size_t read_bytes;

	MSG_DEBUG_( "      offset", "%d", (int)offset );
	MSG_DEBUG_( "      size", "%d", (int)size );

	struct file_handler *f_handler;// = (struct file_handler *)(fi->fh);
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );
	MSG_DEBUG_( "fi->fh", "%lu", fi->fh );
	MSG_DEBUG_( "fd", "%d", f_handler->fd );

	int fd = f_handler->fd;

	read_bytes = pread( fd, buf, size, offset );

	//if( () == -1 ) {
	//	perror( msg_getProgramName() );
	//	//return 0;
	//}

	MSG_DEBUG_( "      read_bytes", "%lu", read_bytes );

	MSG_OPSTAT_SUMMARY();
	return read_bytes;
}

int d2op_release( 	const char 					*path,
							struct fuse_file_info 	*fi	 )
{
	//char *full_path = updatePath( path );
	MSG_OPSTAT_VERBOSE("RELEASE", path);
	//relesePath( full_path );

	struct file_handler *f_handler; // = (struct file_handler *)(fi->fh);
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );


	close( f_handler->fd );
	free( f_handler );

	MSG_OPSTAT_SUMMARY();
	return 0; // ignored by FUSE
}

/* file modify operations */

int d2op_create( 	const char 					*path,
							mode_t				 mode,
							struct fuse_file_info 	*fi 		)
{
	MSG_OPSTAT_VERBOSE("CREATE", path);

	char *full_path = strbuff_setFullPath( op_str_buff, path );

	struct file_handler *f_handler = malloc( sizeof (struct file_handler) );

	if( (f_handler->fd = open( full_path, fi->flags, mode )) == -1 ) {
		free( f_handler );

		return -1;
	}

	chown( full_path, buf_str_uid, buf_str_gid );

	memcpy( &(fi->fh), &f_handler, sizeof( struct file_handler * ) );

	MSG_OPSTAT_SUMMARY();
	return 0;
}

int d2op_write( 	const char 					*path,
						const char 					*buf,
						size_t 						 size,
						off_t 						 offset,
						struct fuse_file_info	*fi 		)
{
	MSG_OPSTAT_VERBOSE("WRITE", path);

	size_t write_bytes;
	struct file_handler *f_handler;
	(void) path;

	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );

	//int fd = f_handler->fd;

	MSG_DEBUG_( "      size", "%lu", size );
	MSG_DEBUG_( "      offset", "%lu", offset );
	MSG_DEBUG_( "    FD", "%d", f_handler->fd );

	write_bytes = pwrite( f_handler->fd, buf, size, offset );
	//if( () == -1 ) {
	//}

	MSG_DEBUG_( 			"      write_bytes", "%lu", write_bytes );

	MSG_OPSTAT_SUMMARY();
	return write_bytes;
}

int d2op_truncate( 	const char 				*path,
							off_t						 length 	)
{
	MSG_OPSTAT_VERBOSE("TRUNCATE", path);

	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val;

	ret_val = truncate( full_path, length );

	//if( () == -1 ) {
	//	perror( msg_getProgramName() );
	//}

	//relesePath( full_path );

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_flush( 	const char 					*path,
					struct fuse_file_info	*fi 		)
{
	MSG_OPSTAT_VERBOSE("FLUSH", path);

	//struct file_handler *f_handler;
	//memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );

	//flush


	MSG_OPSTAT_SUMMARY();
	return 0;
}

int d2op_fsync(		const char 				*path,
					int 					 isdatasync,
					struct fuse_file_info 	*fi	) {

	MSG_OPSTAT_VERBOSE("FSYNC", path);

	int ret_val;
	struct file_handler *f_handler;

	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );

	ret_val = (isdatasync != 0) ? fdatasync(f_handler->fd) :
									fsync(f_handler->fd);


	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_fsyncdir(	const char 				*path,
					int 					 isdatasync,
					struct fuse_file_info 	*fi	) {

	MSG_OPSTAT_VERBOSE("FSYNCDIR", path);

	int ret_val = 0;
	int fd;
	struct dir_handler *d_handler;

	memcpy( &d_handler, &(fi->fh), sizeof ( struct dir_handler * ) );
	fd = dirfd(d_handler->dir_ptr);

	ret_val = (isdatasync != 0) ? fdatasync(fd) : fsync(fd);

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_unlink( const char *path )
{
	MSG_OPSTAT_VERBOSE("UNLINK", path);

	int ret_val;
	char *full_path = strbuff_setFullPath( op_str_buff, path );

	ret_val = unlink( full_path );
	//if( () == -1 ) {
	//	perror( msg_getProgramName() );
	//}

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_rename( const char *src_path, const char *dest_path )
{
	MSG_OPSTAT2_VERBOSE("RENAME", src_path, dest_path);

	int ret_val;

	char *_src_full_path, *src_full_path, *dest_full_path;

	_src_full_path = strbuff_setFullPath( op_str_buff, src_path );

	src_full_path = malloc( (sizeof (char)) * (strlen(_src_full_path) + 1) );

	strcpy(src_full_path, _src_full_path);

	dest_full_path = strbuff_setFullPath( op_str_buff, dest_path );

	ret_val = rename( src_full_path, dest_full_path );
	//if( () == -1 ) {
	//	perror( msg_getProgramName() );
	//}

	free(src_full_path);

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_mknod(const char *path, mode_t mode, dev_t rdev) {
	MSG_OPSTAT_VERBOSE("MKNOD", path);

	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val;

	if( mode & S_IFREG ) {
		MSG_DEBUG( 	"      Regular file" );
	} else if( mode & S_IFCHR ) {
		MSG_DEBUG( 	"      Character device" );
	} else if( mode & S_IFBLK ) {
		MSG_DEBUG( 	"      Block device" );
	} else if( mode & S_IFIFO ) {
		MSG_DEBUG( 	"      FIFO" );
	} else if( mode & S_IFSOCK ) {
		MSG_DEBUG( 	"      Socket" );
	}

	ret_val = mknod(full_path, mode, rdev);

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

/* attributes read operations */

int d2op_getattr(	const char 	*path,
					struct stat *stbuf	)
{
	MSG_OPSTAT_VERBOSE("GETATTR", path);

	//char *full_path = updatePath( path );
	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val = 0;

	//memset( stbuf, 0, sizeof(struct stat) );

	if( lstat( full_path, stbuf ) != 0 ) {
		ret_val = -ENOENT;
	}

	// overwrite uis and gid
	stbuf->st_uid = buf_mnt_uid;
	stbuf->st_gid = buf_mnt_gid;

	//relesePath( full_path );

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_fgetattr(	const char 			*path,
							struct stat *stbuf,
							struct fuse_file_info	*fi	)
{
	MSG_OPSTAT_VERBOSE("FGETATTR", path);

	int ret_val = 0;

	struct file_handler *f_handler;
	memcpy( &f_handler, &(fi->fh), sizeof ( struct file_handler * ) );

	if( fstat( f_handler->fd, stbuf ) != 0 ) {
		ret_val = -ENOENT;
	}

	// overwrite uis and gid
	stbuf->st_uid = buf_mnt_uid;
	stbuf->st_gid = buf_mnt_gid;

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_chmod( const char *path, mode_t mode )
{
	MSG_OPSTAT_VERBOSE("CHMOD", path);

	char *full_path = strbuff_setFullPath( op_str_buff, path );
	int ret_val;

	ret_val = chmod( full_path, mode );

	//if( () == -1 ) {
	//	perror( msg_getProgramName() );
	//}

	//relesePath( full_path );

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_chown( const char *path, uid_t uid, gid_t gid)
{
	MSG_OPSTAT_VERBOSE("CHOWN", path);

	int ret_val = 0;
	// TODO: check if fs is read only
	// if so: return EROFS;

	// if( uid == buf_mnt_uid && gid == buf_mnt_gid )
	{
		// remap permissions:
		ret_val = chown(
			strbuff_setFullPath( op_str_buff, path ),
			buf_str_uid, buf_str_gid);
	} //else {
	//	ret_val = EPERM;
	//}

	// lack of permisions to change owner
	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_utimens( const char *path, const struct timespec tv_am[2] )
{
	MSG_OPSTAT_VERBOSE("UTIMENS", path);

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

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_access(const char *path, int mask) {
	MSG_OPSTAT_VERBOSE("ACCESS", path);

	int ret_val;

	ret_val = access(strbuff_setFullPath( op_str_buff, path ), mask);

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}


// link read/modify operations:
int d2op_readlink(const char *path, char *linkbuf, size_t size) {
	MSG_OPSTAT_VERBOSE("READLINK", path);

	ssize_t linkbuf_len;

	linkbuf_len = readlink(strbuff_setFullPath( op_str_buff, path ), linkbuf, size);

	if(linkbuf_len == size) {
		return EINVAL;
	}

	linkbuf[linkbuf_len] = '\0';


	MSG_OPSTAT_SUMMARY();
	return 0;
}

int d2op_symlink(const char *from, const char *to) {
	MSG_OPSTAT2_VERBOSE("SYMLINK", from, to);

	char *full_path = strbuff_setFullPath( op_str_buff, to );
	int ret_val;

	if( (ret_val = symlink(from, full_path)) != 0 ) {
		return ret_val;
	}

	lchown( full_path, buf_str_uid, buf_str_gid );

	MSG_OPSTAT_SUMMARY();
	return ret_val;
}

int d2op_link(const char *from, const char *to) {
	MSG_OPSTAT2_VERBOSE("LINK", from, to);

	MSG_OPSTAT_SUMMARY();
	return EPERM;
}

/*************************************************************/
