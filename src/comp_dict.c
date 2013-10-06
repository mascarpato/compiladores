/**
 * Compiladores 2013/2
 * 
 * Group: Marcelo Brandalero, Diego Chim, Mauricio Altieri.
 */

#include "../include/comp_dict.h"

	// Deprecated after changed to fixed size.
// void dict_augment(Dict *dict);
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

DictItem *dict_insert(Symbol_t symbol, char *key, int occLine) {
	int i=0, found = 0;

	//printf("Inserindo (%s) de tipo (%d) no dic!\n", key, symbol.symType);

		// Will try to insert the symbol at position pos
	int pos = dict_hashentry(key) % dict->max_size;
	
// 		// Augments dictionary size if it is full

	if (dict->size >= dict->max_size) {
// 		dict_augment(dict); 		// Deprecated
		printf("Symbol table has grown bigger than its limits. aborting...\n");
		abort();
	}

		// Gets insertion position. Position pos may be taken, so will try to insert
		// at next available position
	found = 0;
	i = pos;
	while (dict->max_size && !found) {
		if (dict->begin[i].valid == 0) {
			found = 1; // Will insert new symbol here.
			dict->size = dict->size+1;
		} else {
			if (strcmp(dict->begin[i].key, key) == 0) { // Will replace old symbol
				found = 2;
			} else
				i++;
		}
		
		// If already scanned whole dictionary(i is now out of bounds) and hasn't
		// found insertion position, goes back to start.
		if (i == dict->max_size)
			i = 0;
	}
	
	// Two cases: Will replace symbol or will leave it be.
	if (found == 1) {
		dict->begin[i].key = malloc(sizeof(char)*strlen(key)+1);
		strcpy(dict->begin[i].key, key);
		dict->begin[i].occLine = occLine;
		dict->begin[i].valid=1;
		
		dict->begin[i].symbol = symbol;
	}
	else {
		// Do nothing
	}
	
	if (key == NULL) {
		printf("Hey. Why are you adding a null key?????");
	}
	
	return &(dict->begin[i]);
}

	// TODO CHECK. Has not been fixed after dictionary changed to fixed size.
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
		dict->begin[i-1].symbol.symType = SYMTYPE_UNDEF;
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

	// Deprecated after changed to fixed size.
// void dict_augment(Dict *dict) {
// 	float augmFactor = 1;
// 	int i=0;
// 	int newSize;
// 	
// // 	printf("------ Expanding...\n \
// // 		------ Current size: %d out of %d\n", dict->size, dict->max_size
// // 	);
// 	DictItem *ptAux = dict->begin;
// 	do {
// 		i++;
// 		newSize = (int) dict->max_size*(augmFactor+(1 / (float) i));
// 		ptAux = realloc(dict->begin, sizeof(DictItem)*newSize);
// 		if (ptAux != NULL)
// 			dict->begin = ptAux;
// 	} while (ptAux == NULL);
// 	
// 	dict->max_size = newSize;
// 	for(i = dict->size; i < newSize; i++)
// 		dict->begin[i].valid = 0;
// // 	printf("------ Expanded.\n \
// // 		------ Current size: %d out of %d\n", dict->size, dict->max_size
// // 	);
// }

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

void *dict_get(char *key) {
	DictItem *ptAux = dict->begin;
	int found = 0, i = 0;
	
	for (i=0; i < dict->max_size && !found; i++) {
		if (dict->begin[i].valid)
			if (strcmp(dict->begin[i].key, key) == 0)
				found = 1;
	}
	
	if (found) {
		switch(dict->begin[i-1].symbol.symType) {
			case SYMTYPE_INT:
				return &dict->begin[i-1].symbol.value.value_int;
			case SYMTYPE_FLOAT:
				return &dict->begin[i-1].symbol.value.value_float;
			case SYMTYPE_CHAR:
				return &dict->begin[i-1].symbol.value.value_char;
			case SYMTYPE_STRING:
				return dict->begin[i-1].symbol.value.value_string;
			case SYMTYPE_BOOL:
				return &dict->begin[i-1].symbol.value.value_bool;
			case SYMTYPE_IDENTIFIER:
				return dict->begin[i-1].symbol.name;
			default:
				return NULL;
		}
	} else
		return NULL;
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

			switch(dict->begin[i].symbol.symType) {
				case SYMTYPE_INT:
					printf("-- data : %d\n", dict->begin[i].symbol.value.value_int);
					printf("-- symbol type : INTEGER \n");
					break;
				case SYMTYPE_FLOAT:
					printf("-- data : %f\n", dict->begin[i].symbol.value.value_float);
					printf("-- symbol type : FLOAT \n");
					break;
				case SYMTYPE_CHAR:
					printf("-- data : %c\n", dict->begin[i].symbol.value.value_char);
					printf("-- symbol type : CHAR \n");
					break;
				case SYMTYPE_STRING:
					printf("-- data : %s\n", dict->begin[i].symbol.value.value_string);
					printf("-- symbol type : STRING \n");
					break;
				case SYMTYPE_BOOL:
					printf(dict->begin[i].symbol.value.value_bool > 0 ? "-- data : true\n" : "-- data : false\n");
					printf("-- symbol type : BOOLEAN \n");
					break;
				case SYMTYPE_IDENTIFIER:
					printf("-- name : %s\n", dict->begin[i].symbol.name);
					printf("-- data : %d (this should be empty)", dict->begin[i].symbol.value.value_int);
					printf("-- symbol type : IDENTIFIER \n");
					break;
				default:
					printf("-- data : undendentified or uninitialized symbol\n");;
			}
			printf("-- occLine : %d\n", dict->begin[i].occLine);
			if (dict->begin[i].key != NULL)
				printf("-- hashval = %d\n", dict_hashentry(dict->begin[i].key));
			printf("--------\n");
		}
	}
}
