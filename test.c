#pragma warning(disable : 4996)

#include "lists.h"
#include <stdlib.h>
#include <stdio.h>

#define debug(...) {\
	fprintf(stderr, __VA_ARGS__);\
	fflush(stderr);\
}

int main() {
	#ifdef DEBUG
		fprintf(stderr, "DEBUG MODE enabled.\n");
	#endif
	List list = makelist(0);
	debug("makelist(0).\n%s\n", printlist(&list, 50));
	placelist((size_t)1, &list, sizeof(char) * 3, "OK");
	debug("placelist(1).\n%s\n", printlist(&list, 50));
	pushlist((size_t)0, &list, sizeof(char) * 4, "OKE");
	debug("pushlist(0).\n%s\n", printlist(&list, 50));
	long int at1 = 14;
	pushlist((size_t)4, &list, sizeof(at1), &at1);
	debug("pushlist(4).\n%s\n", printlist(&list, 50));
	rmlist(3, &list);
	debug("rmlist(3).\n%s\n", printlist(&list, 50));
	pushlist((size_t)6, &list, sizeof(at1), &at1);
	debug("pushlist(6).\n%s\n", printlist(&list, 50));
	singlify(&list);
	debug("singlify.\n%s\n", printlist(&list, 50));
	trimlist(&list);
	debug("trimlist.\n%s\n", printlist(&list, 50));
	debug("scanindex(%p): %ld\n", &at1, scanindex(&at1, &list));
}