#include <stdio.h>
#include <assert.h>
#include "../include/comp_dict.h"

int main (int argc, char *argv[]) {
	int i, j, maxsize;
	Dict *dict, *dict2;
	
	char str1[10];
	char str2[10];
	
// 	for (i = 1; i < 20; i++) {
	for (i = 1; i < 10; i++) {
		maxsize = i*100;
		printf("\n\n%d th run: trying for a size of %d words\n", i, maxsize);
		dict = dict_create(maxsize);
		assert(dict != NULL);
		assert(dict_getsize(dict) == 0);
		assert(dict_getmaxsize(dict) == maxsize);
		for(j = 0; j < 2*maxsize; j++) {
			sprintf(str1, "key %d", j);
			sprintf(str2, "data %d", j);
			
			//printf("Adding new(%d) value  to a dict with size (%d, %d)\n",j, dict->size, dict_getmaxsize(dict));
			dict_insert(dict,str1, str2, 0);
			assert(dict_getsize(dict) == j+1);
		}
		
		//printf("--Scanning dictionary--\n");
		//for(j = 0; j < dict_getsize(dict); j++)
			//printf("Element(%d): <%s, %s>\n", j, dict->begin[j].key, dict->begin[j].data);
		
		dict = dict_terminate(dict);
	}
	
	dict2 = dict_create(20);
	assert(dict_getsize(dict2) == 0);
	assert(dict_getmaxsize(dict2) == 20);
	dict_insert(dict2, "key1", "data1",0);
	dict_insert(dict2, "key2", "data2",0);
	dict_insert(dict2, "key3", "data3",0);
	assert(dict_getsize(dict2) == 3);
	assert(dict_getmaxsize(dict2) == 20);
	assert(strcmp(dict_get(dict2, "key1"), "data1") == 0);
	assert(strcmp(dict_get(dict2, "key2"), "data2") == 0);
	assert(strcmp(dict_get(dict2, "key3"), "data2") != 0);
	
	dict_remove(dict2, "key2");
	dict_insert(dict2, "key2new", "data2new",0);
	assert(dict_get(dict2, "key2") == NULL);
	assert(strcmp(dict_get(dict2, "key2new"), "data2new") == 0);
	assert(dict2->size == 3);
	dict2 = dict_terminate(dict2);
	printf("It worked. be happy");
}
