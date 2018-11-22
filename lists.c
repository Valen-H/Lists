#include "lists.h"

extern List makelist(register size_t length) {
	List list;
	list.length = length;
	list.lengths = (size_t*)calloc(list.length, sizeof(size_t));
	list.data = calloc(list.length, sizeof(void*));
	
	return list;
}

extern void placelist(register size_t index, List* list, register size_t size, void* data) {
	if (index >= list->length) {
		register size_t plen = list->length;
		list->length = index + 1;
		list->lengths = (size_t*)realloc(list->lengths, list->length * sizeof(size_t));
		list->data = (void**)realloc(list->data, list->length * sizeof(void*));
		
		for (; plen < list->length; plen++) {
			list->lengths[plen] = 0;
			list->data[plen] = NULL;
		}
	}
	
	char** to = (char**)(list->data + index);
	list->lengths[index] = size;
	*to = (char*)data;
}
