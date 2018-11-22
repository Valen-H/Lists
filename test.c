#include "lists.h"
#include <stdio.h>

int main() {
	List list = makelist(1);
	placelist(2, &list, 3, "12");
	printf("%lu-%lu\n", sizeof(list), list.length);
	printf("%s\n", (char*)list.data[2]);
}
