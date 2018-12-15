#pragma once
	
	#include <stddef.h>
	#include <stdbool.h>
	
	#define rmlist removelist
	#define singlify singlifylist
	
	typedef struct List {
		size_t length,
			*lengths;
		signed long* ids;
		void** data;	
	} List;

	extern List makelist(size_t initsize);
	extern bool initlist(List* list, size_t initsize);
	extern bool extendlist(signed long by, List * list);
	extern bool placelist(size_t index, List * list, size_t size, const void * data);
	extern bool pushlist(size_t index, List * list, size_t length, const void * data);
	extern bool removelist(size_t index, List * list);
	extern size_t singlifylist(List * list);
	extern size_t trimlist(List * list);
	extern char * printlist(List * list, size_t andData);
	extern signed long scanindex(const void * ptr, const List * list);
	extern signed long customscan(bool (* scanindex)(const void * curptr, const void * tarptr), const void * ptr, const List * list);

	/**
	 * placelist(list.length, ...)  --  equiv to js push
	*/
