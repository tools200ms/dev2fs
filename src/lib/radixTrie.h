/* radixTrie.h --- Header file for radixTrie.c.
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


#ifndef _TEAFS_ORG_LIB_RADIX_TRIE_H_
#define _TEAFS_ORG_LIB_RADIX_TRIE_H_


typedef struct value_wrapper
{
	void *value_ptr;
}
ValueWrapper;

#define INIT_VALUE_WRAPPER( val_warpper ) \
					{if( (val_warpper = malloc( sizeof (ValueWrapper) )) == NULL ) return NULL; \
					val_warpper->value_ptr = NULL;}

typedef struct radix_trie_node
{
	char *sub_key;
	struct radix_trie_node *next, *subset;
	ValueWrapper *value;
}
RadixTrieNode;


/**
	Creates RadixTrieNode that can be used as root node for redix trie
*/
RadixTrieNode *rdxTrie_init();


/**
	Destroys redix trie starting from RadixTrieNode *rtn. To destroy the whole 
	structure the root node should be given.
	destroy_fcn is called to destroy value assuciated with a key, if NULL is passed
	values asociated with keys remains allocated.
	destroy_fcn is called before freeing the pointer.
*/
void rdxTrie_destroy( RadixTrieNode *root_node, void (* destroy_fcn)(void *) );


/**
	Prints the keys and it's associated values. Key are printed in an alphabetical order,.
	print_fcn is used to print key's value, if NULL is given only keys are printed.
*/
void rdxTrie_print( RadixTrieNode *root_node, void (* print_fcn)(void *, void *), void *args );

int rdxTrie_count( RadixTrieNode *root_node );

int rdxTrie_copyToTable( RadixTrieNode *root_node, void **values, int size );

/**
	Returns the node associated with given key, if given key is not occupied, 
	the approperiate new entry is created and returned.
	The calling function is in charge to set el pointer of a 
	RadixTrieNode.
*/
ValueWrapper *rdxTrie_insert( RadixTrieNode *root_node, char *key );


/**
	finds a value associated with a given key, if key doesn't exists BULL value is 
	returned
*/
ValueWrapper *rdxTrie_find( RadixTrieNode *root_node, char *key );

ValueWrapper *rdxTrie_findByPosition( RadixTrieNode *root_node, int pos );

void *rdxTrie_findInPath( RadixTrieNode *root_node, char *path, char **shift );

/**
	removes node with a given key from a radix trie, if given key doesn't exists 
	NULL value is returned
*/
void *rdxTrie_remove( RadixTrieNode *root_node, char *key );

#endif // _TEAFS_ORG_LIB_RADIX_TRIE_H_
