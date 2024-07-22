/* messages.c --- Functions for message printing.
 * Copyright (C) 2012 Mateusz Piwek
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


#include <stdio.h>

#include "../dev2fs.h"
#include "messages.h"


/**
	
*/
static bool verbose_messages = false;

void msgSetVerboseMode()           { verbose_messages = true; }

inline bool msgIsVerboseModeSet()   { return verbose_messages; }
