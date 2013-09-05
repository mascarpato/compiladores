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

DictItem *dict_insert(char *key, int sym_type, int occLine) {
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
	strcpy(dict->begin[i-1].key, key);
	dict->begin[i-1].occLine = occLine;
	dict->begin[i-1].valid=1;
	dict->size = dict->size+1;
	
	switch(sym_type) {
		case SYMTYPE_INT_LIT:
			symSetInt(&dict->begin[i-1].symbol, atoi(key));
			//dict->begin[i-1].symbol.symbol.data_int = atoi(key);
			//dict->begin[i-1].symbol.symType = SYMTYPE_INT_LIT;
			break;
		case SYMTYPE_FLOAT_LIT:
			symSetFloat(&dict->begin[i-1].symbol, atof(key));
			break;
		case SYMTYPE_CHAR_LIT:
			symSetChar(&dict->begin[i-1].symbol, *key);
			break;
		case SYMTYPE_STRING_LIT:
			symSetString(&dict->begin[i-1].symbol, key);
			break;
		case SYMTYPE_BOOL_LIT:
			symSetBool(&dict->begin[i-1].symbol, *key == 't' ? 1 : 0);
			break;
		case SYMTYPE_IDENT:
			symSetIdent(&dict->begin[i-1].symbol, key);
			break;
		default:
			dict->begin[i-1].symbol.symType = SYMTYPE_UNDEF;
			break;
	}
	
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
			case SYMTYPE_INT_LIT:
				return &dict->begin[i-1].symbol.symbol.data_int;
			case SYMTYPE_FLOAT_LIT:
				return &dict->begin[i-1].symbol.symbol.data_float;
			case SYMTYPE_CHAR_LIT:
				return &dict->begin[i-1].symbol.symbol.data_char;
			case SYMTYPE_STRING_LIT:
				return dict->begin[i-1].symbol.symbol.data_string;
			case SYMTYPE_BOOL_LIT:
				return &dict->begin[i-1].symbol.symbol.data_bool;
			case SYMTYPE_IDENT:
				return dict->begin[i-1].symbol.symbol.ident;
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
				case SYMTYPE_INT_LIT:
					printf("-- data : %d\n", dict->begin[i].symbol.symbol.data_int);
					printf("-- symbol type : INTEGER \n");
					break;
				case SYMTYPE_FLOAT_LIT:
					printf("-- data : %f\n", dict->begin[i].symbol.symbol.data_float);
					printf("-- symbol type : FLOAT \n");
					break;
				case SYMTYPE_CHAR_LIT:
					printf("-- data : %c\n", dict->begin[i].symbol.symbol.data_char);
					printf("-- symbol type : CHAR \n");
					break;
				case SYMTYPE_STRING_LIT:
					printf("-- data : %s\n", dict->begin[i].symbol.symbol.data_string);
					printf("-- symbol type : STRING \n");
					break;
				case SYMTYPE_BOOL_LIT:
					printf(dict->begin[i].symbol.symbol.data_bool > 0 ? "-- data : true\n" : "-- data : false\n");
					printf("-- symbol type : BOOLEAN \n");
					break;
				case SYMTYPE_IDENT:
					printf("-- data : %s\n", dict->begin[i].symbol.symbol.ident);
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
