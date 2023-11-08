/* devfs.h --- Basic definitions for Dev2FS.
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


#ifndef _DEV2FS_200MS_NET__DEV2FS_H_
#define _DEV2FS_200MS_NET__DEV2FS_H_

#define FUSE_USE_VERSION 26


#define DEV2FS_AUTHORS "Mateusz Piwek <Mateusz@teafs.org>"

/*
	Versioning schema:
	major<.1alpha|.2beta|.3rc|4>.minor
	
	major:
		Indicates 'major' changes that is:
		- implementaion of a new functionalities
		- API changes
	minor:
		Indicates changes that include:
		- bug fixes
		- code optimization
		- or rewrites
	status:
		Code's development status:
		.1alpha - alpha
		.2beta - beta
		.3rc - release candidate (RC)
		4 - final release

		In alpha and beta states code undergoes major changes, while RC
		and later '4' (final) is fixed for mainor only updates.

	- The minor field can contain '-' (minus) sign folowed by a number.
	  It indicates that code at this stage doesn't support all
	  functionalities planned for release with a given major number.
	  For instance 1.1alpha.3-2 says that a certain functionalities
	  scheduled for 1.4.0 are not implemented yet, while 1.1.alpha.7-1
	  tells that the progress in this field has been made since
	  1.1alpha.3-2, and this release is more functional but still not
	  complete.
	  NOTE 1: The '-' sign can be used only for alpha and beta releases.
	  NOTE 2: The fianal releas ends with '4', e.g. 2.0.4, 2.1.4 is a
	  code after 'minor' updates.
*/
#define DEV2FS_VERSION "1.2beta.5"

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
