/* devfs.h --- Basic definitions for TeaFS.
 * Copyright (C) 2012, 2023 Mateusz Piwek
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


#ifndef _DEV2FS_200MS_NET__DEV2FS_H_
#define _DEV2FS_200MS_NET__DEV2FS_H_

#define FUSE_USE_VERSION 26


#define DEV2FS_AUTHORS "Mateusz Piwek <Mateusz@teafs.org>"

/*
	Version number notation:
	major.minor.maintenance.status<-revision> <ALPHA|BETA|RC>
	
	major:
		Indicates the major functionality changes
	minor:
		Indicates the minor functionality changes
	maintenance:
		Points out code changes that are not bug fixes or 
		functionality extensions, this value should be 
		increased in the case of e.g. code optimization 
		or rewrites
	status:
		Code's development status:
		0 - alpha
		1 - beta
		2 - release candidate
		3 - final release (RC)
		3x, 4 - final release with bug fixes, 
				  e.g. 3.2 minor bugs fixes, 
				  4 - major bug fixes
		In alpha and beta states the code is under development, when release 
		candidate is reached only bug-fixes can be applied.
		If the maintenance number changes, the status should be changed 
		to alpha, beta or RC as the change might trigger unexpected bugs.
	<-revision>
		When code is in alpha, beta or RC states the svn's revision number is 
		added.
	<ALPHA|BETA|RC>
		it's added to emphasize the no stable program's status.
	
	- Please note that for final releases only major.minor.status schema applies.
	
	- The major and minor numbers indicates the functionalities of released 
	  software, however the minor field can contain '-' (minus) sign as well.
	  It indicates that this release doesn't support all functionalities planned 
	  for a certain major.minor version but the implementation started.
	  For instance major.minor fields marked as 1.0-2 says that the functionalities 
	  scheduled for 1.0 are not implemented yet, while 1.0-1 indicates that the 
	  progress has been done since 1.0-2 and this release is more functional but 
	  still not complete.
	  The '-' sign can be used only for alpha and beta releases.
*/
#define DEV2FS_VERSION "0.1.0.0-ALPHA"

/* Detect if 'musl libc' is used, from:
 * https://stackoverflow.com/questions/58177815/how-to-actually-detect-musl-libc
 */
#define _GNU_SOURCE
#include <features.h>
#ifndef __USE_GNU
    #define __MUSL__
#endif
#undef _GNU_SOURCE /* don't contaminate other includes unnecessarily */


#endif // _DEV2FS_200MS_NET__DEV2FS_H_
