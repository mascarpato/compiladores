#include "../include/comp_dict.h"

void dict_augment(Dict *dict);

Dict *dict_create(int size) {
	Dict *dict = malloc(sizeof(Dict));
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

Dict *dict_insert(Dict *dict, char *key, char *data, int occLine) {
	int i=0, found = 0;
	
		// Checks if key is already on the table
	for (i = 0; i < dict->max_size && !found; i++) {
		if (dict->begin[i].valid)
			if (strcmp(dict->begin[i].key, key) == 0) {
				// Already on the table.
				found = 1;
			}
	}
	if (found)
		return;
	else {
			// Data is not on the dictionary. Should insert.
	
		if (dict->size == dict->max_size) {
			dict_augment(dict);
		}
		
		found = 0;
			// Gets insertion position
		for (i = 0; i < dict->max_size && !found; i++) {
			if (dict->begin[i].valid == 0) {
				found = 1;
			}
		}
		
		dict->begin[i-1].key = malloc(sizeof(char)*strlen(key)+1);
		dict->begin[i-1].data = malloc(sizeof(char)*strlen(data)+1);
		dict->begin[i-1].occLine = occLine;
		dict->begin[i-1].valid=1;
		
		strcpy(dict->begin[i-1].key, key);
		strcpy(dict->begin[i-1].data, data);
		dict->size = dict->size+1;
		
		return dict;
	}
}

Dict *dict_remove(Dict *dict, char *key) {
	int i=0, found = 0;
	
		// Gets item position
	for (i = 0; i < dict->max_size && !found; i++) {
		if (dict->begin[i].valid)
			if (strcmp(dict->begin[i].key, key) == 0)
				found = 1;
	}
	
	if (found) {
		free(dict->begin[i-1].key);
		free(dict->begin[i-1].data);
		dict->begin[i-1].key = NULL;
		dict->begin[i-1].data = NULL;
		dict->begin[i-1].valid = 0;

		dict->size--;
		return dict;
	} else
		return dict;
}

Dict *dict_terminate(Dict *dict) {
	int i;
	
	for (i = 0; i < dict->max_size; i++) {
		//printf("Calling free twice here!\n");
		if (dict->begin[i].valid) {
			free(dict->begin[i].key);
			free(dict->begin[i].data);
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
	
	printf("------ Expanding...\n \
		------ Current size: %d out of %d\n", dict->size, dict->max_size
	);
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
	printf("------ Expanded.\n \
		------ Current size: %d out of %d\n", dict->size, dict->max_size
	);
}

int dict_getsize(Dict *dict) {
	if (dict != NULL)
		return dict->size;
	else
		return -1;
}
int dict_getmaxsize(Dict *dict) {
	if (dict != NULL)
		return dict->max_size;
	else
		return -1;
}

char *dict_get(Dict *dict, char *key) {
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
		return NULL;
}

// 
// /* Create a key-value pair. */
// entry_t *dict_newpair( char *key, char *value ) {
// 	entry_t *newpair;
// 	
// 	if( ( newpair = malloc( sizeof( entry_t ) ) ) == NULL ) {
// 		return NULL;
// 	}
// 	
// 	if( ( newpair->key = strdup( key ) ) == NULL ) {
// 		return NULL;
// 	}
// 	
// 	if( ( newpair->value = strdup( value ) ) == NULL ) {
// 		return NULL;
// 	}
// 	
// 	newpair->next = NULL;
// 	
// 	return newpair;
// }
// 
// /* Insert a key-value pair into a hash table. */
// void dict_set( hashtable_t *hashtable, char *key, char *value ) {
// 	int bin = 0;
// 	entry_t *newpair = NULL;
// 	entry_t *next = NULL;
// 	entry_t *last = NULL;
// 	
// 	bin = dict_hash( hashtable, key );
// 	
// 	next = hashtable->table[ bin ];
// 	
// 	while( next != NULL && next->key != NULL && strcmp( key, next->key ) > 0 ) {
// 		last = next;
// 		next = next->next;
// 	}
// 	
// 	/* There's already a pair. Let's replace that string. */
// 	if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {
// 		
// 		free( next->value );
// 		next->value = strdup( value );
// 		
// 		/* Nope, could't find it. Time to grow a pair. */
// 	} else {
// 		newpair = dict_newpair( key, value );
// 		
// 		/* We're at the start of the linked list in this bin. */
// 		if( next == hashtable->table[ bin ] ) {
// 			newpair->next = next;
// 			hashtable->table[ bin ] = newpair;
// 			/* We're at the end of the linked list in this bin. */
// 		} else if ( next == NULL ) {
// 			last->next = newpair;
// 			/* We're in the middle of the list. */
// 		} else {
// 			newpair->next = next;
// 			last->next = newpair;
// 		}
// 	}
// }
// 
// /* Retrieve a key-value pair from a hash table. */
// char *dict_get( hashtable_t *hashtable, char *key ) {
// 	int bin = 0;
// 	entry_t *pair;
// 	
// 	bin = dict_hash( hashtable, key );
// 	
// 	/* Step through the bin, looking for our value. */
// 	pair = hashtable->table[ bin ];
// 	while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) > 0 ) {
// 		pair = pair->next;
// 	}
// 	
// 	/* Did we actually find anything? */
// 	if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
// 		return NULL;
// 		
// 	} else {
// 		return pair->value;
// 	}
// }