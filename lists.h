#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdlib.h>
	
	typedef struct List {
        	size_t* lengths;
        	size_t length;
        	void** data;
        } List;
	
        extern List makelist(register size_t length);
        extern void placelist(size_t index, List* list, size_t size, void* data);
	
#endif
