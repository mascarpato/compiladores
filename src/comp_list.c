#include "../include/comp_list.h"

ListNode *list_create() {
  return NULL;
}

ListNode *list_prepend(ListNode *begin, void *data) {
  ListNode *newNode = malloc(sizeof(ListNode));
  
  newNode->data = data;

  if (begin == NULL) {
    newNode->next = NULL;
  } else {
    newNode->next = begin;
  }

  return newNode;
}

ListNode *list_append(ListNode *begin, void *data) {
  ListNode *newNode = malloc(sizeof(ListNode));
  ListNode *p;
  
  newNode->data = data;
  newNode->next = NULL;

  if (begin == NULL) {
	return newNode;
  } else {
	p = begin;
	while(begin->next != NULL)
		p = p->next;
	
    p->next = newNode;
	return begin;
  }
}

ListNode *list_get(ListNode *begin, int index) {
	int i = 0, found = 1;
	ListNode *p = begin;
	
	while (i < index && found) {
		if (p != NULL) {
			p = p->next;
			i++;
		} else {
			found = 0;
		}
	}
	
	return p;
}

ListNode *list_remove(ListNode *begin, void *data) {
  ListNode *p = begin;
  ListNode *pPrev = NULL;
  int found = 0;

  while (p != NULL && !found) {
    if (p->data == data)
      found = 1;
    else {
      pPrev = p;
      p = p->next;
    }
  }

  if (found && pPrev == NULL) {
	// Means element is in the first position
    pPrev = p;
	p = p->next;
    free(pPrev);
    return p;
  } else if (found && pPrev != NULL) {
	// Means element is in the middle or last position
    pPrev->next = p->next;
    free(p);
    return begin;
  } else {
    return begin;
  }
}

ListNode *list_remove_rdata(ListNode *begin, void *data) {
  ListNode *p = begin;
  ListNode *pPrev = NULL;
  int found = 0;

  while (p != NULL && !found) {
    if (p->data == data)
      found = 1;
    else {
      pPrev = p;
      p = p->next;
    }
  }

  if (found && pPrev == NULL) {
	// Means element is in the first position
    pPrev = p;
	p = p->next;
	if (pPrev->data != NULL) {
		free(pPrev->data);
		pPrev->data = NULL;
	}
    free(pPrev);
    return p;
  } else if (found && pPrev != NULL) {
	// Means element is in the middle or last position
    pPrev->next = p->next;
	if (p->data != NULL) {
		free(p->data);
		p->data = NULL;
	}
    free(p);
    return begin;
  } else {
    return begin;
  }
}

int list_contains(ListNode *begin, void *data) {
	ListNode *p = begin;
	ListNode *pPrev = NULL;
	
	int found = 0;
	
	while (p != NULL && !found) {
		if (p->data == data)
			found = 1;
		else {
			pPrev = p;
			p = p->next;
		}
	}
	
	if (found) {
		return 1;
	} else {
		return 0;
	}
}

ListNode *list_concatenate(ListNode *list1, ListNode *list2) {
  ListNode *p, *pAux;

  p = list1;
  if (p == NULL) {
    return list2;
  } else {
	  while (p->next != NULL) {
		p = p->next;
	  }
	  
	  p->next = list2;
  }
  
  return list1;
}

ListNode *list_terminate(ListNode *begin) {
	ListNode *p = begin;
	ListNode *pAux;
	
	if (p == NULL) {
		return NULL;
	} else {
		while (p != NULL) {
			pAux = p;
			p = p->next;
			
			free(pAux);
			pAux = NULL;
		}
	}
}

int list_size(ListNode *begin) {
	int i = 0;
	ListNode *p = begin;
	
	while (p!=NULL) {
		p=p->next;
		i++;
	}
	
	return i;
}