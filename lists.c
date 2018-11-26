#include "lists.h"

extern List makelist(register size_t length) {
	List list;
	list.length = length;
	list.lengths = (size_t*)calloc(list.length, sizeof(size_t));
	list.data = (void**)calloc(list.length, sizeof(void*));
	
	return list;
}

extern char placelist(register size_t index, List* list, register size_t size, void* data) {
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
			void* tm = (void*)(list->data[plen]);
			tm = NULL;
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

extern void* rmlist(register size_t index, List* list) {
	void* ret = list->data[index];
	
	for (; index < list->length; index++) {
		if (index + 1 < list->length) {
			list->data[index] = list->data[index + 1];
			list->lengths[index] = list->lengths[index + 1];
		} else {
			char* tm = (char*)(list->data[index]);
			tm = NULL;
			list->lengths[index] = 0;
		}
	}
	
	list->length--;
	list->data = (void**)realloc(list->data, sizeof(void*) * list->length);
	list->lengths = (size_t*)realloc(list->lengths, sizeof(size_t) * list->length);
	
	return ret;
}

extern char pushlist(register size_t index, List* list, size_t size, void* data) {
	
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
	
	for (register size_t i = list->length - 2; i >= index; i--) {
		list->data[i + 1] = list->data[i];
		list->lengths[i + 1] = list->lengths[i];
	}
	
	return placelist(index, list, size, data);
}
