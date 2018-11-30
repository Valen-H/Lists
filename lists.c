#include "lists.h"
#include <stdbool.h>
#include <stdio.h>

extern List makelist(const register size_t length) {
	List list;
	list.length = length;
	list.lengths = (size_t*)calloc(list.length, sizeof(size_t));
	list.data = (void**)calloc(list.length, sizeof(void*));
	
	return list;
}

extern char placelist(register size_t index, List* const list, const register size_t size, const void* const data) {
	if (index >= list->length) {
		
		size_t* tmp = (size_t*)realloc(list->lengths, (index + 1) * sizeof(size_t));
		void** ttmp = (void**)realloc(list->data, (index + 1) * sizeof(void*));
		
		if (tmp == NULL || ttmp == NULL) {
			free(tmp);
			free(ttmp);
			return -1;
		}
		
		register size_t plen = list->length;
		
		for (; plen < index + 1; plen++) {
			list->lengths[plen] = 0;
			list->data[plen] = NULL;
		}
		
		free(list->lengths);
		free(list->data);
		list->length = index + 1;
		list->lengths = tmp;
		list->data = ttmp;
		
	}
	
	char** restrict to = (char**)(list->data + index);
	list->lengths[index] = size;
	*to = (char*)data;
	
	return 0;
}

extern void* rmlist(register size_t index, List* const list) {
	void* ret = list->data[index];
	
	for (; index < list->length; index++) {
		if (index + 1 < list->length) {
			list->data[index] = list->data[index + 1];
			list->lengths[index] = list->lengths[index + 1];
		} else {
			list->data[index] = NULL;
			list->lengths[index] = 0;
		}
	}
	
	list->length--;
	list->data = (void**)realloc(list->data, sizeof(void*) * list->length);
	list->lengths = (size_t*)realloc(list->lengths, sizeof(size_t) * list->length);
	
	return ret;
}

extern char pushlist(register size_t index, List* const list, const size_t size, const void* const data) {
	
	size_t* tmp = (size_t*)realloc(list->lengths, sizeof(size_t) * (list->length + 1));
	void** ttmp = (void**)realloc(list->data, sizeof(void*) * (list->length + 1));
	
	if (tmp == NULL || ttmp == NULL) {
		free(tmp);
		free(ttmp);
		return -1;
	}
	
	list->length++;
	free(list->lengths);
	free(list->data);
	list->data = ttmp;
	list->lengths = tmp;

	if (list->length > 1) {
		for (register signed long int i = list->length - 2; i >= index; i--) {
			list->data[i + 1] = list->data[i];
			list->lengths[i + 1] = list->lengths[i];
		}
	}
	
	return placelist(index, list, size, data);
}

extern void singlify(List* const list) {
	
	void** ndata = (void**)calloc(list->length, sizeof(void*));
	register size_t nsize = 0;
	size_t* nlen = (size_t*)calloc(list->length, sizeof(size_t));
	
	for (register size_t i = 0; i < list->length; i++) {
		bool pass = true;
		for (register size_t ii = 0; ii < nsize; ii++) {
			if (ndata[ii] == list->data[i]) {
				pass = false;
				break;
			}
		}
		if (pass) {
			ndata[nsize] = list->data[i];
			nlen[nsize++] = list->lengths[i];
		}
	}
	ndata = (void**)realloc(ndata, nsize * sizeof(void*));
	nlen = (size_t*)realloc(nlen, nsize * sizeof(size_t));
	list->data = ndata;
	list->length = nsize;
	list->lengths = nlen;
}
