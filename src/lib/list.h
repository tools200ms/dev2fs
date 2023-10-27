/* list.h --- Header file for list.c.
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


#ifndef _DEV2FS_200MS_NET__LIB_LIST_H_
#define _DEV2FS_200MS_NET__LIB_LIST_H_

/** minimum value is 2 (one pointer must be
	 reserved for data, one to indicate next
	 table)
*/
#define UNIV_LIST_SIZE 100

/**
	intendent to work as sorted list or FIFO
*/

typedef struct list
{
	void **first_ptr, **last_ptr;
	void **end_of_first_table, **end_of_last_table;
}
List;

List *	init_list();
void 	destroy_list( List *list );

List *	list_write( List *list, void *el );
void *	list_read( List *list );

int 	list_isEmpty( List *list );

#endif // _DEV2FS_200MS_NET__LIB_LIST_H_
