/* tags.c --- Tag structure manipulation functions.
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


#include <stdio.h>
#include "tags.h"

#include "messages.h"


static void tags_print_tag( void *tag, void *args )
{
	fprintf( (FILE *)args, "/%s/\n" , (char *)tag );
}

void tags_print( FILE *stream, RadixTrieNode *tags_set )
{
	//int i;
	fprintf( stream, MSG_TAGS_INFO );

	rdxTrie_print( tags_set, tags_print_tag, stream );
}
