#include "lists.h"
#include <stdio.h>

int main() {
	List list = makelist(1);
	printf("\nmakelist(1).\nlistbytes: %lub\nlength: %lu\n", sizeof(list), list.length);
	placelist(2, &list, sizeof("12"), "12");
	printf("placelist(2,,%lu,\"12\").\nlength: %lu\n", sizeof("12"), list.length);
	printf("\n%s\n", printlist(&list));
	printf("data[2]: \"%s\"\n", (char*)list.data[2]);
	printf("rmlist(2): \"%s\".\n", (char*)rmlist(2, &list));
	printf("length: %lu\n", list.length);
	printf("\n%s\n", printlist(&list));
	unsigned int i = 14u;
	placelist(4, &list, sizeof(i), &i);
	printf("placelist(4,,%lu,14u).\nlength: %lu\nlengths[4]: %lub\ndata[4]: %uu\n", sizeof(i), list.length, list.lengths[4], *(unsigned int*)list.data[4]);
	printf("rmlist(3): %p.\nlength: %lu\ndata[3]: %uu\ndata[4]: %p\nlengths[3]: %zub\n", rmlist(3, &list), list.length, *(unsigned int*)list.data[3], list.data[4], list.lengths[3]);
	pushlist(4, &list, sizeof(i), &i);
	printf("\n%s\n", printlist(&list));
	printf("pushlist(4,,%lu,14u).\nlength: %lu\nlengths[4]: %lub\ndata[3]: %uu\ndata[4]: %uu\n", sizeof(i), list.length, list.lengths[4], *(unsigned int*)list.data[3], *(unsigned int*)list.data[4]);
	singlify(&list);
	printf("singlify.\nlength: %lu\nlengths[1]: %lub\ndata[1]: %c\nlengths[0]: %lu\ndata[0]: %p\n", list.length, list.lengths[1], *(unsigned int*)list.data[1], list.lengths[0], list.data[0]);
	printf("\n%s\n\n", printlist(&list));
}
