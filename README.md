# Lists.h  
  
This library offers the utility of runtime variable length arrays that support items of variable type.  
  
> AKA: an auto-resizable array that can hold many different types simultaneously!  
  
## Usage  
  
```C
List list = makelist(5); /* Initial size. */
/* makelist(register size_t iniSize) */
placelist(3, &list, sizeof("10"), (void*)"10"); /* Resizes the array pointer to fit the data. */
/* extern char placelist(register size_t index, List* list, register size_t size, void* data) */
/* The 'size' parameter is obsolete and can be used as ID for the user's convenience. */
int i = 10;
void* ip = &i;
pushlist(2, &list, sizeof(i), ip); /* Same as 'placelist' but instead of overriding the data, it pushes all items after the index one slot to the right. */
printf("%s", (char*)list.data[4]); /* 12 */
rmlist(3, &list); /* Removes item at specified index, shifts all items after the index one slot left and returns the removed item pointer. */
/* extern void* rmlist(register size_t index, List* list) */
scanindex(ip, &list); /* index of first occurence of 'ip' */
/* extern size_t scanindex(const void * const data, const List * const list) */

/**
	typedef struct List {
		size_t length;
		size_t* lengths; /* Size of each item */
		void** data; /* Each item... */
	} List;
*/
```  
