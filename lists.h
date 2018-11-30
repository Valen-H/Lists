#pragma once

#include <stdlib.h>
	
	typedef struct List {
        	size_t * lengths;
        	size_t length;
        	void * * data;
        } List;
	
        extern List makelist(const register size_t length);
        extern char placelist(register size_t index, List * const list, const register size_t size, const void * const data);
        /*^ 'size' can be used as ID instead */
        extern void * rmlist(register size_t index, List* const list);
        extern char pushlist(register size_t index, List * const list, const register size_t size, const void * const data);
        extern void singlify(List * const list);
        extern size_t scanindex(const void * const ptr, const List * const list);
