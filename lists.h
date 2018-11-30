#pragma once

#include <stdlib.h>
	
	typedef struct List {
		size_t * lengths,
			* ids,
			length;
		void * * data;
	} List;
	
        extern List makelist(const register size_t length);
        extern signed char placelist(register size_t index, List * const list, const register size_t size, const void * const data);
        /*^ 'size' can be used as ID instead */
        extern void * rmlist(register size_t index, List * const list);
        extern signed char pushlist(register size_t index, List * const list, const register size_t size, const void * const data);
        extern void singlify(List * const list);
        extern signed long int scanindex(const void * const ptr, const List * const list);
        extern char * printlist(const List * const list);
