#include "lists.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

static size_t intlen(signed long int value) {
	size_t l = !value;
	
	while (value >= 1) {
		l++;
		value /= 10;
	}
	
	return l;
}

/*static char* dechex(signed long int num) {
	size_t len = intlen(num);
	char* ret = (char*)calloc(len + 3, sizeof(char));
	sprintf(ret, "0x%lx", num);
	return ret;
}*/

static size_t hexsize(const void* const ptr) {
	char* ret = (char*)calloc(30, sizeof(char));
	register size_t i;
	
	sprintf(ret, "%p", ptr);
	
	for (i = 0; i < 30; i++) {
		if (ret[i] == '\0') break;
	}
	
	free(ret);
	
	return i;
}

extern char* printlist(const List* const list) {
	char* tmp = (char*)calloc(intlen(SIZE_MAX) * 3 + hexsize(list) + 11, sizeof(char)),
		* ret = (char*)calloc(intlen(SIZE_MAX) * 3 + hexsize(list) + 11, sizeof(char) * list->length);
		
	for (register size_t i = 0; i < list->length; i++) {
		sprintf(tmp, "%lu,%lu: %p (%lu) - \n", i, list->ids[i], list->data[i], list->lengths[i]);
		strcat(ret, tmp);
	}
	
	free(tmp);
	
	return ret;
}

extern List makelist(const register size_t length) {
	List list;
	list.length = length;
	list.lengths = (size_t*)calloc(list.length, sizeof(size_t));
	list.ids = (size_t*)calloc(list.length, sizeof(size_t));
	list.data = (void**)calloc(list.length, sizeof(void*));
	
	return list;
}

extern signed char placelist(register size_t index, List* const list, const register size_t size, const void* const data) {
	if (index >= list->length) {
		
		size_t* tmp = (size_t*)realloc(list->lengths, (index + 1) * sizeof(size_t)),
			* tttmp = (size_t*)realloc(list->ids, (index + 1) * sizeof(size_t));
		void** ttmp = (void**)realloc(list->data, (index + 1) * sizeof(void*));
		
		if (tmp == NULL || ttmp == NULL || tttmp == NULL) {
			free(tmp);
			free(ttmp);
			free(tttmp);
			return -1;
		}
		
		register size_t plen = list->length;
		
		free(list->lengths);
		free(list->data);
		free(list->ids);
		list->length = index + 1;
		list->lengths = tmp;
		list->data = ttmp;
		list->ids = tttmp;
		
		for (; plen < list->length; plen++) {
			list->lengths[plen] = 0;
			list->data[plen] = NULL;
			list->ids[plen] = 0;
		}
		
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
			list->ids[index] = list->ids[index + 1];
		} else {
			list->data[index] = NULL;
			list->lengths[index] = 0;
			list->ids[index] = 0;
		}
	}
	
	list->length--;
	list->data = (void**)realloc(list->data, sizeof(void*) * list->length);
	list->lengths = (size_t*)realloc(list->lengths, sizeof(size_t) * list->length);
	list->ids = (size_t*)realloc(list->ids, sizeof(size_t) * list->length);
	
	return ret;
}

extern signed char pushlist(register size_t index, List* const list, const size_t size, const void* const data) {
	
	size_t* tmp = (size_t*)realloc(list->lengths, sizeof(size_t) * (list->length + 1)),
		* tttmp = (size_t*)realloc(list->ids, (index + 1) * sizeof(size_t));
	void** ttmp = (void**)realloc(list->data, sizeof(void*) * (list->length + 1));
	
	if (tmp == NULL || ttmp == NULL || tttmp == NULL) {
		free(tmp);
		free(ttmp);
		free(tttmp);
		return -1;
	}
	
	list->length++;
	free(list->lengths);
	free(list->data);
	free(list->ids);
	list->data = ttmp;
	list->lengths = tmp;
	list->ids = tttmp;

	if (list->length > 1) {
		for (register signed long int i = list->length - 2; i >= index; i--) {
			list->data[i + 1] = list->data[i];
			list->lengths[i + 1] = list->lengths[i];
			list->ids[i + 1] = list->ids[i];
		}
	}
	
	return placelist(index, list, size, data);
}

extern void singlify(List* const list) {
	
	void** ndata = (void**)calloc(list->length, sizeof(void*));
	register size_t nsize = 0;
	size_t* nlen = (size_t*)calloc(list->length, sizeof(size_t)),
		* nnlen = (size_t*)calloc(list->length, sizeof(size_t));
	
	for (register size_t i = 0; i < list->length; i++) {
		bool pass = true;
		for (register size_t ii = 0; ii < nsize; ii++) {
			if (ndata[ii] == list->data[i] && nlen[ii] == list->lengths[i] && nnlen[ii] == list->ids[i]) {
				pass = false;
				break;
			}
		}
		if (pass) {
			ndata[nsize] = list->data[i];
			nlen[nsize] = list->lengths[i];
			nnlen[nsize++] = list->ids[i];
		}
	}
	
	ndata = (void**)realloc(ndata, nsize * sizeof(void*));
	nlen = (size_t*)realloc(nlen, nsize * sizeof(size_t));
	nnlen = (size_t*)realloc(nnlen, nsize * sizeof(size_t));
	
	free(list->data);
	free(list->lengths);
	free(list->ids);
	
	list->data = ndata;
	list->length = nsize;
	list->lengths = nlen;
	list->ids = nnlen;
}

extern signed long int scanindex(const void* const ptr, const List* const list) {
	
	for (register signed long int i = 0; i < list->length; i++) {
		if (list->data[i] == ptr) return i;
	}
	
	return -1;
}
