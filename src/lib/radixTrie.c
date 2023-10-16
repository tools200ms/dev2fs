/* radixTrie.c --- Implementation of radix trie for character strings 
                   search.
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

#include <string.h>


#include "radixTrie.h"

#include "messages.h"



/* static functions */

static inline RadixTrieNode *rdxTrie_insert_new_node( RadixTrieNode *prev_node, RadixTrieNode *cur_node )
{
	RadixTrieNode *new_node;

	if( ( new_node = malloc( sizeof (RadixTrieNode) ) ) == NULL )
		return NULL;

	prev_node->next = new_node;
	new_node->next = cur_node;
	new_node->subset = NULL;
	new_node->sub_key = NULL;
	
	return new_node;
}

static RadixTrieNode *rdxTrie_insert_node_to_level( RadixTrieNode *cur_node, char k )
{
	RadixTrieNode *prev_node;
	
	for( 	prev_node = cur_node, cur_node = cur_node->next; 
			cur_node->sub_key != NULL; 
			prev_node = cur_node, cur_node = cur_node->next )
	{
		if( k == *((*cur_node).sub_key) )
			return cur_node;
		else if( k < *((*cur_node).sub_key) ) //add new node between two existing nodes
			break;
	}

	return rdxTrie_insert_new_node( prev_node, cur_node );
}

/* **************** */


RadixTrieNode *rdxTrie_init()
{
	RadixTrieNode *root_node;

	if( (root_node = malloc( sizeof (RadixTrieNode) )) != NULL )
	{
		//memset( root_node, 0, sizeof (RadixTrieNode) );
		root_node->sub_key = NULL;
		root_node->subset = NULL;
		root_node->value = NULL;

		root_node->next = root_node;
	}
	
	return root_node;
}


void rdxTrie_destroy( RadixTrieNode *root_node, void (* destroy_fcn)(void *) )
{
	RadixTrieNode *cur_node = root_node->next;
	
	while( cur_node->sub_key != NULL )
	{
		if( cur_node->value != NULL )
		{
			if( destroy_fcn != NULL )
				destroy_fcn( cur_node->value->value_ptr );
			
			free( cur_node->value );
		}
		
		if( cur_node->subset != NULL )
		{
			// recursive call
			rdxTrie_destroy( cur_node->subset, destroy_fcn );
		}
		
		free( cur_node->sub_key );
		
		root_node = cur_node->next;
		free( cur_node );
		cur_node = root_node;
	}
	
	free( cur_node );
}


void rdxTrie_print( RadixTrieNode *root_node, void (* print_fcn)(void *, void *), void *args )
{
	for( root_node = root_node->next; root_node->sub_key != NULL; root_node = root_node->next )
	{
		MSG_DEBUG_STR( "sub key: ", root_node->sub_key );
		
		if( root_node->value != NULL )
			print_fcn( root_node->value->value_ptr, args );
		
		if( root_node->subset != NULL )
		{
			MSG_DEBUG_STR( "====> calling rdxTrie_print_node for: ", root_node->sub_key );
			// recursive call
			rdxTrie_print( root_node->subset, print_fcn, args );
			MSG_DEBUG_STR( "<==== back in: ", root_node->sub_key );
		}
	}
}

int rdxTrie_count( RadixTrieNode *root_node )
{
	int count = 0;
	
	for( root_node = root_node->next; root_node->sub_key != NULL; root_node = root_node->next )
	{
		if( root_node->value != NULL )
			++count;
		
		if( root_node->subset != NULL )
			// recursive call
			count += rdxTrie_count( root_node->subset );
	}
	
	return count;
}

int rdxTrie_copyToTable( RadixTrieNode *root_node, void **values, int size )
{
	int count = 0, ret_val;
	
	for( root_node = root_node->next; root_node->sub_key != NULL; root_node = root_node->next )
	{
		if( root_node->value != NULL )
		{
			if( count != size )
				values[count++] = root_node->value->value_ptr;
			else return -1;
		}
		
		if( root_node->subset != NULL )
		{
			// recursive call
			if( (ret_val = rdxTrie_copyToTable( root_node->subset, values + count, size - count )) == -1 )
				return -1;
			
			count += ret_val;
		}
	}
	
	return count;
}


ValueWrapper *rdxTrie_insert( RadixTrieNode *root_node, char *key )
{
	RadixTrieNode *cur_node, *new_node;
	char *node_key;
	
	insert_key:
	cur_node = rdxTrie_insert_node_to_level( root_node, *key );

	if( cur_node->sub_key != NULL )
	{
		/**
			possible combinations:
			         I:   II:  III:
			sub_key: a  | a  | ad
			key:    	a  | ad | a
		*/
		
		for( ++key, node_key = cur_node->sub_key + 1; *node_key == *key; ++key, ++node_key )
		{
			// option I, key is the same string as sub_str
			if( *key == '\0' )
			{
				if( cur_node->value == NULL )
					INIT_VALUE_WRAPPER( cur_node->value );
					//cur_node->value = malloc( sizeof (ValueWrapper) );
				
				return cur_node->value;
			}
		}
		
		if( *node_key == '\0' )
		{
			// option II, key is a suffix of sub_key
			if( cur_node->subset == NULL )
			{
				if( (cur_node->subset = root_node = rdxTrie_init()) == NULL )
					return NULL;
				
				new_node = rdxTrie_insert_new_node( root_node, root_node );
				new_node->sub_key = strdup( key );
				
				INIT_VALUE_WRAPPER( new_node->value );
				return new_node->value;// = malloc( sizeof (ValueWrapper) );
			}
			
			root_node = cur_node->subset;

			goto insert_key;
			//return rdxTrie_add_node( cur_node->subset, key + key_idx );
		}
		
			// option III, key is a substring of sub_key
			// break sub_key in the point where the last common character is
			// e.g. adso, adfw, brakes to ad, sa and fw
			// breaking string always triggers creation of a new level
		
		root_node = rdxTrie_init();
		
		new_node = rdxTrie_insert_new_node( root_node, root_node );
		
		new_node->sub_key = strdup( node_key );
		new_node->value = cur_node->value;
		new_node->subset = cur_node->subset;

		*node_key = '\0';
		cur_node->subset = root_node;
		cur_node->value = NULL;
		
		if( *key != '\0' )
		{
			cur_node = rdxTrie_insert_node_to_level( root_node, *key );
			cur_node->sub_key = strdup( key );
			
		//	INIT_VALUE_WRAPPER( cur_node->value );
		//	return cur_node->value;// = malloc( sizeof (ValueWrapper) );
			//return rdxTrie_insert_node( root_node, key );
		}
		
		INIT_VALUE_WRAPPER( cur_node->value );
		return cur_node->value; // = malloc( sizeof (ValueWrapper) );
	}


	cur_node->sub_key = strdup( key );
	
	INIT_VALUE_WRAPPER( cur_node->value );
	
	return cur_node->value; // = malloc( sizeof (ValueWrapper) );
}


ValueWrapper *rdxTrie_find( RadixTrieNode *root_node, char *key )
{
	RadixTrieNode *cur_node;
	char *node_key;

	search_level:
	for( 	cur_node = root_node->next; 
			cur_node->sub_key != NULL; 
			cur_node = cur_node->next 	)
	{
		if( *(cur_node->sub_key) == *key )
		{
			for( ++key, node_key = cur_node->sub_key + 1; *key == *node_key ; ++key, ++node_key )
			{
				if( *key == '\0' ) // key matches
					return cur_node->value;
			}
			
			if( *node_key == '\0' ) // chance to find key in next level
			{
				if( (root_node = cur_node->subset) != NULL )
					goto search_level;
			}
			
			break; // no match
		}
		else if( *(cur_node->sub_key) > *key ) 
		// no chance to find the key in this level, thus no such key in trie at all
			break;
	}
	
	return NULL;
}


static ValueWrapper *rdxTrie_findByPosition_priv( RadixTrieNode *root_node, int *pos )
{
	ValueWrapper *val_wrapp;
	
	for( root_node = root_node->next; root_node->sub_key != NULL; root_node = root_node->next )
	{
		if( root_node->value != NULL )
		{
			if( (*pos)-- == 0 )
			{
				MSG_DEBUG_STR( "sub key: ", root_node->sub_key );
				return root_node->value;
			}
		}
		
		if( root_node->subset != NULL )
		{
			//MSG_DEBUG_STR( "====> calling rdxTrie_print_node for: ", root_node->sub_key );
			// recursive call
			if( (val_wrapp = rdxTrie_findByPosition_priv( root_node->subset, pos )) != NULL )
				return val_wrapp;
		}
	}
	
	return NULL;
}


ValueWrapper *rdxTrie_findByPosition( RadixTrieNode *root_node, int pos )
{
	return rdxTrie_findByPosition_priv( root_node, &pos );
}


void *rdxTrie_findInPath( RadixTrieNode *root_node, char *path, char **shift )
{
	RadixTrieNode *cur_node;
	char *node_key;

	search_level:
	for( 	cur_node = root_node->next; 
			cur_node->sub_key != NULL; 
			cur_node = cur_node->next 	)
	{
		if( *(cur_node->sub_key) == *path )
		{
			for( ++path, node_key = cur_node->sub_key + 1; *path == *node_key ; ++path, ++node_key )
			{
				if( *path == '\0' ) // key matches
				{
					*shift = path;
					return cur_node->value->value_ptr;
				}
			}
			
			if( *node_key == '\0' ) // chance to find key in next level
			{
				if( *path == '/' )
				{
					*shift = path + 1;
					return cur_node->value->value_ptr;
				}
				
				if( (root_node = cur_node->subset) != NULL )
					goto search_level;
			}
			
			break; // no match
		}
		else if( *(cur_node->sub_key) > *path )
		// no chance to find the key in this level, thus no such key in trie at all
			break;
	}
	
	
	for( ; *path != '\0'; ++path )
	{
		if( *path == '/' )
		{
			++path;
			break;
		}
	}

	*shift = path;
	return NULL;
}

void *rdxTrie_remove( RadixTrieNode *root_node, char *key )
{
	RadixTrieNode *prev_node, *cur_node, *prev_root_node = root_node;
	char *node_key, *new_sub_key;
	void *value_ptr = NULL;
	int cnt;


	insert_one_level:
	for( 	prev_node = root_node, cur_node = root_node->next; 
			cur_node->sub_key != NULL; 
			prev_node = cur_node, cur_node = cur_node->next )
	{
		if( *(cur_node->sub_key) == *key )
		{
			for( ++key, node_key = cur_node->sub_key + 1; *key == *node_key ; ++key, ++node_key )
			{
				if( *key == '\0' ) // key matches
				{
					if( cur_node->value == NULL )
						return NULL;
					
					// removes value
					value_ptr = cur_node->value->value_ptr;
					free( cur_node->value );
					
					cur_node->value = NULL;
					
					if( cur_node->subset != NULL )
					{
						// another levels are attached, checks if the need for 
						// mearging exists
						prev_node = cur_node;
						cur_node = cur_node->subset;
						
						if( cur_node->next->next == cur_node )
						// exactly one key in current level exists, it needs to be merged with previous level
						// UP merging (key is moved to the next level, empty level is removed)
						{
							cur_node = cur_node->next;
							cnt = strlen( prev_node->sub_key ) + strlen( cur_node->sub_key ) + 1;
							new_sub_key = malloc( sizeof (char) * cnt );
							
							strcpy( new_sub_key, prev_node->sub_key );
							strcat( new_sub_key, cur_node->sub_key );
							
							free( prev_node->sub_key );
							free( cur_node->sub_key );
							
							prev_node->subset = cur_node->subset;
							prev_node->sub_key = new_sub_key;
							prev_node->value = cur_node->value;
							
							free( cur_node->next );
							free( cur_node );
						}
					}
					else
					{
						// node with removed key was terminating node, 
						// the key node can be safely removed
						prev_node->next = cur_node->next;
						free( cur_node->sub_key );
						free( cur_node );
						
						if( prev_node->next == prev_node )
						// the current level is empty
						{
							if( prev_root_node != root_node )
							// it's not the first level, so the level can be safely removed
							{
								for( 	prev_root_node = prev_root_node->next; 
										prev_root_node->subset != root_node; 
										prev_root_node = prev_root_node->next );
								
								prev_root_node->subset = NULL;
								
								free( prev_node );
							}
						}
						else if( prev_node->next->next == prev_node )
						// current level has exactly one key
						{
							if( prev_root_node != root_node )
							{
								for( 	prev_root_node = prev_root_node->next; 
										prev_root_node->subset != root_node; 
										prev_root_node = prev_root_node->next )
										MSG_DEBUG_STR( "LEVEL realased: ", prev_root_node->sub_key );
							
								if( prev_root_node->value == NULL )
								// the node has no value and the only one key is in the next level, 
								// DOWN merging (the only key will be moved and merged one level down)
								{
									cur_node = root_node->next;
									MSG_DEBUG_STR( "LEVEL realased: ", cur_node->sub_key );
								
									cnt = strlen( prev_root_node->sub_key ) + strlen( cur_node->sub_key ) + 1;
									new_sub_key = malloc( sizeof (char) * cnt );
							
									strcpy( new_sub_key, prev_root_node->sub_key );
									strcat( new_sub_key, cur_node->sub_key );
							
							
									free( prev_root_node->sub_key );
									free( cur_node->sub_key );
							
									prev_root_node->subset = NULL;
									prev_root_node->sub_key = new_sub_key;
									prev_root_node->value = cur_node->value;
							
									free( root_node );
									free( cur_node );
								}
							}
						}
					}
					
					return value_ptr;
				}
			}
			
			if( *node_key == '\0' ) // chance to find key in next level
			{
				if( cur_node->subset != NULL )
				{
					prev_root_node = root_node;
					root_node = cur_node->subset;
					goto insert_one_level;
				}
			}
			
			break; // no match
		}
		else if( *(cur_node->sub_key) > *key )
		// no chance to find the key in this level, thus no such key in trie at all
			break;
	}
	
	return NULL;
}

