/*!
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maurício Altieri.
 * 
 * @file comp_dict.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "comp_symbols.h"

#ifndef __COMP_DICT
#define __COMP_DICT

extern struct comp_dict_t  *dict;

/*!\struct comp_dict_item_t
 * \brief A dictionary item, consisting of a key and a data value.
 *  
 * Both key and data are char* (strings). An extra valid element is for control and should not be messed with.
 */
typedef struct comp_dict_item_t {
	short valid; //!< Do not mess with me.
	char *key; //! Symbol key
	Symbol_t symbol; //! Symbol - name, type, value. 
	int occLine; //! Occurrence line
} DictItem;

/*! \brief Dictionary Data Type.
 * 
 * The dictionary is implemented as a dynamically allocated array of @ref comp_dict_item_t. The initial size must be given when creating the dictionary. It automatically doubles its size everytime it is full.
 * Here is implemented a stack of dictionaries.
 * 
 */
typedef struct comp_dict_t {
	int size; //!< Do not mess with me.
	int max_size; //!< Neither with me
	DictItem *begin;
	struct comp_dict_t *prev;
} Dict;

/*! \brief Creates a dictionary.
 * 
 * Usage: dict = dict_create(initial_size);
 * 
 * The initial size must not be equal to the final size of the dictionary, as it automatically doubles its size when it is full.
 */
extern Dict *dict_create(int size);

/*! \brief Inserts a new (key, data) pair in the dictionary.
 * 
 * Usage: dict = dict_insert(dict, "key", "data", "occurrence line");
 */
extern DictItem *dict_insert(Symbol_t symbol, char *key, int occLine);

/*! \brief Gets the word with specified key.
 * 
 * Usage: word = dict_get(dict, "key");
 * 
 * Note: Returns NULL if no word has the specified key.
 */
extern DictItem *dict_get(char *key);

/*! \brief Removes the word with key (key) from the dictionary. Nothing happens if the word is not found.
 * 
 * Usage: dict = dict_remove(dict, "key");
 * 
 */
extern Dict *dict_remove(char *key);

/*! \brief Removes the dictionary from the of the stack.
 * 
 * Usage: dict = dict_pop();
 * 
 */
extern Dict *dict_pop();

/*! \brief Finishes the use of the dictionary. Call this when you are done.
 * 
 * Usage: dict = dict_terminate(dict)
 */
extern Dict *dict_terminate();

/*! \brief Gets the number of used slots in the dictionary.
 */
extern int dict_getsize();

/*! \brief Gets the current maximum size of the dictionary.
 */
extern int dict_getmaxsize();

/*! \brief Prints to stdout a human-readable representation of the dictionary. */
extern void dict_print();
#endif