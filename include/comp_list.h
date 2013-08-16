/*! 
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Maurício Altieri.
 * 
 * @file comp_list.h
 * 
 */

#include <stdio.h>
#include <stdlib.h>

/*! \brief Basic Node element used in the list.
 * 
 */
typedef struct comp_list_t {
  void *data;
  struct comp_list_t *next;
} ListNode;

/*! \brief Creates an empty list.
 * 
 * 	Usage: list = list_create();
 */
extern ListNode *list_create();

/*! \brief Adds an element to the beginning of the list.
 * 
 * 	Usage: list = list_prepend(list, &data);
 */
extern ListNode *list_prepend(ListNode *begin, void *data);

/** \brief Adds an element to the end of the list.
 * 
 * 	Usage: list = list_append(list, &data);
 */
extern ListNode *list_append(ListNode *begin, void *data);

/** \brief Gets an element (the node containing the element, actually) at position index. 
 * 
 * 	Usage: node = list_get(list, index);
 * 
 * 	\returns The node, if index is valid, or NULL if index is invalid.
 */
extern ListNode *list_get(ListNode *begin, int index);

/** \brief Removes the node containing data from the list.
 * 
 * 	Usage: list = list_remove(list, &data);
 * Note: This function does not remove(free) the data element itself. For this, use <list_remove_rdata>"()".
 */
extern ListNode *list_remove(ListNode *begin, void *data);

/** \brief Removes the node containing data from the list.
 * 
 * 	Usage: list = list_remove(list, &data);
 * Note: this function also removes(frees) the data element itself.
 */
extern ListNode *list_remove_rdata(ListNode *begin, void *data);

/** \brief Checks if the list contains the data element.
 * 
 * 	Usage: list_contains(list, &data);
 * \returns 0 if false, 1 otherwise.
 */
extern int list_contains(ListNode *begin, void *data);

/** \brief Returns a new list made by concatenating list1 and list2.
 * 
 * 	Usage: newlist = list_concatenate(list1, list2);
 * Notes:
 * 1. No guarantee is given on the order the lists are concatenated.
 * 2. Care must be taken when after freeing the lists, as double frees may ocurr.
 */
extern ListNode *list_concatenate(ListNode *list1, ListNode *list2);

/** \brief Terminates a list, freeing all nodes.
 * 
 * 	Usage: list = list_terminate(list);
 */
extern ListNode *list_terminate(ListNode *begin);

/** \brief Returns the size of the list.
 */
extern int list_size(ListNode *begin);

/** \brief Prints a human-readable representation of the list.
 */
extern void list_print(ListNode *begin);