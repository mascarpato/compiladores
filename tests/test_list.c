#include <stdio.h>
#include <assert.h>
#include "../include/comp_list.h"

int vector[] = {0, 1, 2, 3, 4, 5};

/** Tests the list function defined in \file comp_list.h
 */
int main() {
  ListNode *list1, *list2;
  ListNode *elem;
  
	// Creates a list1. Must return null.
  list1 = list_create();
  assert(list1 == NULL);
  
	// Adds 0 and 1 to the list1
  list1 = list_prepend(list1, &vector[0]);
  list1 = list_prepend(list1, &vector[1]);
  assert(list_contains(list1, &(vector[0])));
  assert(list_contains(list1, &(vector[1])));
  
	// Adds 2 and 3 to the list1
  list1 = list_prepend(list1, &vector[2]);
  assert(list_contains(list1, &vector[2]));
  list1 = list_prepend(list1, &vector[3]);
  assert(list_contains(list1, &vector[3]));
  
	// Removes 0 and 3 from the list1
  list1 = list_remove(list1, &vector[3]);
  assert(list_contains(list1, &(vector[0])));
  assert(list_contains(list1, &(vector[1])));
  assert(list_contains(list1, &(vector[2])));
  assert(!list_contains(list1, &(vector[3])));
  list1 = list_remove(list1, &vector[0]);
  assert(!list_contains(list1, &(vector[0])));
  assert(list_contains(list1, &(vector[1])));
  assert(list_contains(list1, &(vector[2])));
  assert(!list_contains(list1, &(vector[3])));
  
	// Creates new empty list
  list2 = list_create();
  // Concatenates to new empty list the previous list.
  // empty list gets all elements of list1 and list1 stays unaltered.
  list2 = list_concatenate(list1, list2);
  assert(!list_contains(list2, &(vector[0])));
  assert(list_contains(list2, &(vector[1])));
  assert(list_contains(list2, &(vector[2])));
  assert(!list_contains(list2, &(vector[3])));
  assert(!list_contains(list1, &(vector[0])));
  assert(list_contains(list1, &(vector[1])));
  assert(list_contains(list1, &(vector[2])));
  assert(!list_contains(list1, &(vector[3])));
  assert(list_size(list2) == 2);
  assert(list_size(list1) == 2);
  
	// List shoulld have now two elements. Will try to get them
  assert(list_get(list2, 0) != NULL);
  assert(list_get(list2, 1) != NULL);
	// Should return null here, because element doesnt exist.
  assert(list_get(list2, 2) == NULL); 
  
  list2 = list_terminate(list2);
  
  // New test ---------------------------------
  ListNode *list3 = list_create();
  list3 = list_append(list3, &vector[0]);
  list3 = list_append(list3, &vector[1]);
  list3 = list_prepend(list3, &vector[2]);
  assert(list_get(list3, 0)->data == &vector[2]);
  assert(list_get(list3, 1)->data == &vector[0]);
  assert(list_get(list3, 2)->data == &vector[1]);
  assert(list_size(list3) == 3);
  list3 = list_remove(list3, &vector[2]);
  list3 = list_remove(list3, &vector[0]);
  assert(list_size(list3) == 1);
  list3 = list_terminate(list3);
  
}