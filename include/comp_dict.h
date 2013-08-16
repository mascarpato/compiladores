/*! 
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maurício Altieri.
 * 
 * @file comp_dict.h
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*!\struct comp_dict_item_t
 * \brief A dictionary item, consisting of a key and a data value.
 *  
 * Both key and data are char* (strings). An extra valid element is for control and should not be messed with.
 */
typedef struct comp_dict_item_t {
	short valid; //!< Do not mess with me.
	char *key; //! Symbol key
	char *data; //! Symbol value
	int occLine; //! Occurrence line
} DictItem;

/*! \brief Dictionary Data Type.
 * 
 * The dictionary is implemented as a dynamically allocated array of @ref comp_dict_item_t. The initial size must be given when creating the dictionary. It automatically doubles its size everytime it is full.
 * 
 */
typedef struct comp_dict_t {
	int size; //!< Do not mess with me.
	int max_size; //!< Neither with me
	DictItem *begin;	
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
 * Usage: dict = dict_insert(dict, "key", "data");
 */
extern Dict *dict_insert(Dict *dict, char *key, char *data, int occLine);

/*! \brief Gets the word with specified key.
 * 
 * Usage: word = dict_get(dict, "key");
 * 
 * Note: Returns NULL if no word has the specified key.
 */
extern char *dict_get(Dict *dict, char *key);

/*! \brief Removes the word with key (key) from the dictionary. Nothing happens if the word is not found.
 * 
 * Usage: dict = dict_remove(dict, "key");
 * 
 */
extern Dict *dict_remove(Dict *dict, char *key);

/*! \brief Finishes the use of the dictionary. Call this when you are done.
 * 
 * Usage: dict = dict_terminate(dict)
 */
extern Dict *dict_terminate(Dict *dict);

/*! \brief Gets the number of used slots in the dictionary.
 */
extern int dict_getsize(Dict *dict);

/*! \brief Gets the current maximum size of the dictionary.
 */
extern int dict_getmaxsize(Dict *dict);

/*! \brief Prints to stdout a human-readable representation of the dictionary. */
extern void dict_print(Dict *dict);