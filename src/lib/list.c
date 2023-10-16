/* list.c --- Implementation of FIFO list.
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
#include <stdlib.h>

#include "list.h"

List *init_list()
{
	List *list = malloc( sizeof (List) );

	if( list == NULL )
		return NULL;

	(*list).first_ptr = (*list).last_ptr = 
				malloc( (sizeof (void **)) * UNIV_LIST_SIZE );
	
	(*list).end_of_first_table = (*list).end_of_last_table = 
				(*list).first_ptr + (UNIV_LIST_SIZE - 1);
	
	*((*list).end_of_last_table) = NULL;

	return list;
}

void destroy_list( List *list )
{
	void **end_of_table, **next_end_of_table;

	end_of_table = (*list).end_of_first_table;

	while( *(end_of_table) != NULL )
	{
		next_end_of_table = *end_of_table; // + (UNIV_LIST_SIZE - 1));
		
		free( end_of_table - (UNIV_LIST_SIZE - 1) );
		end_of_table = next_end_of_table;
	}

	free( end_of_table - (UNIV_LIST_SIZE - 1) );
}


/**
	Add element to FIFO list.
	list - pointer to the list to with element should be added
	el - pointer to element
	On return list pointer is returned or NULL if 
	error occured
*/
List *list_write( List *list, void *el )
{
	*((*list).last_ptr++) = el;

	if( (*list).last_ptr == (*list).end_of_last_table )
	{
		void **new_table = malloc( (sizeof (void **)) * UNIV_LIST_SIZE );
		
		if( new_table == NULL )
			return NULL;
		
		(*list).last_ptr = new_table;
		*((*list).end_of_last_table) = new_table;
		(*list).end_of_last_table = new_table + (UNIV_LIST_SIZE - 1);
		*((*list).end_of_last_table) = NULL;
	}

	return list;
}

void *list_read( List *list )
{
	if( (*list).first_ptr != (*list).last_ptr )
	{
		if( (*list).first_ptr == (*list).end_of_first_table )
		{
			(*list).first_ptr = *((*list).end_of_first_table);
			free( (*list).end_of_first_table - (UNIV_LIST_SIZE - 1) );
			
			(*list).end_of_first_table = (*list).first_ptr + (UNIV_LIST_SIZE - 1);
		}
		
		return *((*list).first_ptr++);
	}
	// queue is empty

	return NULL;
}

int list_isEmpty( List *list )
{
	return (*list).first_ptr == (*list).last_ptr;
}
