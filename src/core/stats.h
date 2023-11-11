/* stats.h --- prototype of statistics of fs operations.
 * Copyright (C) 2023 Mateusz Piwek
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


#ifndef _DEV2FS_200MS_NET__CORE_STATS_H_
#define _DEV2FS_200MS_NET__CORE_STATS_H_

#include <time.h>

typedef struct stats {
	struct timespec t_s, t_e;
} Stats;

Stats *stat_init();
void stat_destroy(Stats *s);

inline void op_start(Stats *s);
inline void op_end(Stats *s);


void stat_opendir();
void stat_readdir();
void stat_releasedir();
void stat_mkdir();
void stat_rmdir();
void stat_open();
void stat_read();
void stat_release();
void stat_create();
void stat_write();
void stat_truncate();
void stat_unlink();
void stat_rename();
void stat_mknod();
void stat_getattr();
void stat_fgetattr();
void stat_chmod();
void stat_chown();
void stat_utimens();
void stat_access();
void stat_flush();
void stat_fsync();
void stat_fsyncdir();
void stat_readlink();
void stat_symlink();
void stat_link();



#endif // _DEV2FS_200MS_NET__CORE_STATS_H_
