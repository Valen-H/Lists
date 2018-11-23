#pragma once

#include <stdlib.h>
	
	typedef struct List {
        	size_t* lengths;
        	size_t length;
        	void** data;
        } List;
	
        extern List makelist(register size_t length);
        extern char placelist(register size_t index, List* list, register size_t size, void* data);
        /*^ 'size' can be used as ID instead */
        extern void* rmlist(register size_t index, List* list);
        extern char pushlist(register size_t index, List* list, register size_t size, void* data);
