/**
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Mauricio Altieri.
 */

#include "../include/comp_dict.h"

void dict_augment(Dict *dict);
int dict_hashentry(char *key); 

Dict *dict;

Dict *dict_create(int size) {
	dict = malloc(sizeof(Dict));
	dict->size = 0;
	dict->max_size = size;
	dict->begin = NULL;
	
	int i;
	DictItem *ptAux;
	
	if (size>1) {
		if ((dict->begin = calloc(sizeof(DictItem),size)) != NULL) {
			dict->max_size = size;
		}
	}
	
	return dict;
}

DictItem *dict_insert(char *key, int data, int occLine) {
	int i=0, found = 0;

	int pos = dict_hashentry(key) % dict->max_size;
	
		// Checks if key is already on the table
		// Marcelo -- Commented out as we now replace the data
		// with the new values.
// 	for (i = pos; i < dict->max_size && !found; i++) {
// 		if (dict->begin[i].valid)
// 			if (strcmp(dict->begin[i].key, key) == 0) {
// 				// Already on the table.
// 				found = 1;
// 			}
// 	}

	// Inserts or replaces data
	if (dict->size == dict->max_size) {
		dict_augment(dict);
	}

	found = 0;
		// Gets insertion position
	for (i = pos; i < dict->max_size && !found; i++) {
		if (dict->begin[i].valid == 0) {
			found = 1; // Will insert new
		} else {
			if (dict->begin[i].key != NULL)
				if (strcmp(dict->begin[i].key, key) == 0) {
					found = 1; // Will replace
					// frees old space in memory. will later alloc again.
					free(dict->begin[i].key);
				}
		}
	}
	
	dict->begin[i-1].key = malloc(sizeof(char)*strlen(key)+1);
	dict->begin[i-1].data = data;
	dict->begin[i-1].occLine = occLine;
	dict->begin[i-1].valid=1;
	
	strcpy(dict->begin[i-1].key, key);
	dict->size = dict->size+1;
	
	return &(dict->begin[i-1]);
}

Dict *dict_remove(char *key) {
	int i=0, found = 0;
	
		// Gets item position
	for (i = 0; i < dict->max_size && !found; i++) {
		if (dict->begin[i].valid)
			if (strcmp(dict->begin[i].key, key) == 0)
				found = 1;
	}
	
	if (found) {
		free(dict->begin[i-1].key);
		dict->begin[i-1].key = NULL;
		dict->begin[i-1].data = 0;
		dict->begin[i-1].valid = 0;

		dict->size--;
		return dict;
	} else
		return dict;
}

Dict *dict_terminate() {
	int i;
	
	for (i = 0; i < dict->max_size; i++) {
		//printf("Calling free twice here!\n");
		if (dict->begin[i].valid) {
			free(dict->begin[i].key);
		}
	}
	free(dict->begin);
	free(dict);
	
	return NULL;
}

void dict_augment(Dict *dict) {
	float augmFactor = 1;
	int i=0;
	int newSize;
	
// 	printf("------ Expanding...\n \
// 		------ Current size: %d out of %d\n", dict->size, dict->max_size
// 	);
	DictItem *ptAux = dict->begin;
	do {
		i++;
		newSize = (int) dict->max_size*(augmFactor+(1 / (float) i));
		ptAux = realloc(dict->begin, sizeof(DictItem)*newSize);
		if (ptAux != NULL)
			dict->begin = ptAux;
	} while (ptAux == NULL);
	
	dict->max_size = newSize;
	for(i = dict->size; i < newSize; i++)
		dict->begin[i].valid = 0;
// 	printf("------ Expanded.\n \
// 		------ Current size: %d out of %d\n", dict->size, dict->max_size
// 	);
}

int dict_getsize() {
	if (dict != NULL)
		return dict->size;
	else
		return -1;
}
int dict_getmaxsize() {
	if (dict != NULL)
		return dict->max_size;
	else
		return -1;
}

int dict_get(char *key) {
	DictItem *ptAux = dict->begin;
	int found = 0, i = 0;
	
	for (i=0; i < dict->max_size && !found; i++) {
		if (dict->begin[i].valid)
			if (strcmp(dict->begin[i].key, key) == 0)
				found = 1;
	}
	
	if (found) {
		return dict->begin[i-1].data;
	} else
		return -1;
}

int dict_hashentry(char *key) {
	int i=0, hash=0;

	if (key == NULL)
		return 0;

		// Yet Another Hash Function
	for(i == 0; i < strlen(key); i++) {
		hash = (hash << 3) + (int) 3*key[i];			
	}

	if (hash > 0)
		return hash;
	else
		return -hash;
}

void dict_print() {
	int i;
	if (dict == NULL) {
		printf("dict == NULL.\n");
		return;
	} else if (dict->begin == NULL) {
		printf("dict->begin == NULL.\n");
		return;
	} else {
		printf("-------- Printing dictionary --------\n");
		for (i = 0; i < dict->max_size; i++) {
			printf("---- Item %d ----\n", i);
			printf("-- valid : %d\n", dict->begin[i].valid);
			if (dict->begin[i].key != NULL)
				printf("-- key : %s\n", dict->begin[i].key);

			printf("-- data : %d\n", dict->begin[i].data);
			printf("-- occLine : %d\n", dict->begin[i].occLine);
			if (dict->begin[i].key != NULL)
				printf("-- hashval = %d\n", dict_hashentry(dict->begin[i].key));
			printf("--------\n");
		}
	}
}
