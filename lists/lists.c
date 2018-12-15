#pragma warning(disable : 4996)

#include "lists.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static const size_t MAXPTRADDRSIZE = 30;

static size_t hexsize(const void* const ptr) {
	char* ret = (char*) calloc(MAXPTRADDRSIZE, sizeof(char));
	register size_t i;

	snprintf(ret, MAXPTRADDRSIZE, "%p", ptr);
	i = strlen(ret);
	free(ret);

	return i; /* EXCLUDES \0 */
}

static char* hexify(void* str, size_t delim) {
	size_t size = delim ? delim : (strlen(str) + 1);
	char* ret = (char*) calloc(size * 4 + 1, sizeof(char)),
		*acc = (char*) calloc(4, sizeof(char));

	memset(ret, 0, size * 3 + 1);

	for (register size_t i = 0; i < size; i++) {
		snprintf(acc, 4, "%02X ", ((unsigned char*) str)[i]);
		if (i) {
			strcat(ret, acc);
		} else {
			strcpy(ret, acc);
		}
	}

	free(acc);

	return ret;
}

static size_t intsize(register long value) {
	register size_t l = !value;

	while (value >= 1) {
		l++;
		value /= 10;
	}

	return l;
}

extern bool initlist(List* list, size_t initsize) {
	list->length = initsize;  /* NO ALLOCING */
	list->lengths = (size_t*)calloc(initsize, sizeof(size_t));
	list->ids = (signed long*)calloc(initsize, sizeof(signed long));
	list->data = (void**)calloc(initsize, sizeof(void*));

	if (list->lengths == NULL || list->ids == NULL || list->data == NULL) {
		return false;
	}

	for (register size_t i = 0; i < list->length; i++) {
		list->lengths[i] = 0;
		list->ids[i] = -1;  /* calloc AUTO ZERO-FILLs! Though, needed for custom-fill... */
		list->data[i] = NULL;
	}

	return true;
}

extern List makelist(size_t initsize) {
	List list;

	initlist(&list, initsize);

	return list;
}

extern bool extendlist(signed long by, List* list) {
	size_t* tmplens = (size_t*) realloc(list->lengths, (list->length + by) * sizeof(size_t));  /* FAIL */
	signed long* tmpids = (signed long*) realloc(list->ids, (list->length + by) * sizeof(signed long));
	void** tmpdat = (void**) realloc(list->data, (list->length + by) * sizeof(void*));
	bool pass[3] = { true, true, true }, all = true;

	if (tmplens == NULL) {
		pass[0] = false;
		all = false;
	}
	if (tmpids == NULL) {
		pass[1] = false;
		all = false;
	}
	if (tmpdat == NULL) {
		pass[2] = false;
		all = false;
	}

	if (!all) {
		if (pass[0]) list->lengths = (size_t*) realloc(tmplens, list->length * sizeof(size_t));
		if (pass[1]) list->ids = (signed long*) realloc(tmpids, list->length * sizeof(signed long));
		if (pass[2]) list->data = (void**) realloc(tmpdat, list->length * sizeof(void*));

		return false;
	}

	list->lengths = tmplens;
	list->ids = tmpids;
	list->data = tmpdat;

	for (register size_t i = list->length; i < list->length + by; i++) {
		list->lengths[i] = 0;
		list->ids[i] = -1;
		list->data[i] = NULL;
	}

	list->length += by;

	return true;
}

extern bool placelist(size_t index, List* list, size_t size, const void* data) {
	if (index >= list->length) {
		if (!extendlist(index - list->length + 1, list)) return false;
	} else if (index < 0) {
		return false;
	}

	list->data[index] = data;
	list->lengths[index] = size;
	list->ids[index] = 0;

	return true;
}

extern bool pushlist(size_t index, List* list, size_t length, const void* data) {
	if (index >= list->length) {
		if (!placelist(index, list, 0, NULL)) return false;
	} else if (index < 0) {
		return false;
	}

	if (!extendlist(1, list)) return false;

	register size_t i;

	for (i = list->length - 2; i > index; i--) {
		list->lengths[i + 1] = list->lengths[i];
		list->ids[i + 1] = list->ids[i];
		list->data[i + 1] = list->data[i];
	}

	list->lengths[i + 1] = 0;
	list->ids[i + 1] = -1;
	list->data[i + 1] = NULL;

	return placelist(index, list, length, data);
}

extern bool removelist(size_t index, List* list) {
	if (index < 0) return false;

	for (register size_t i = index + 1; i < list->length; i++) {
		list->data[i - 1] = list->data[i];
		list->lengths[i - 1] = list->lengths[i];
		list->ids[i - 1] = list->ids[i];
	}

	list->data[list->length - 1] = NULL;
	list->lengths[list->length - 1] = 0;
	list->ids[list->length - 1] = -1;

	return extendlist(-1, list);
}

extern size_t singlifylist(List* list) {
	size_t remlen = 0;

	for (register size_t i = 0; i < list->length; i++) {
		for (register size_t t = 0; t < i; t++) {
			if (list->data[i] == list->data[t]) {
				removelist(i--, list);
				remlen++;
				break;
			}
		}
	}

	return remlen;
}

extern size_t trimlist(List* list) {
	size_t remlen = 0;

	for (register size_t i = 0; i < list->length; i++) {
		if (list->data[i] == NULL) {
			removelist(i--, list);
			remlen++;
		}
	}

	return remlen;
}

extern char* printlist(List* list, size_t andData) {
	const size_t cnt = (intsize(list->length) + intsize(SIZE_MAX) * 2 + hexsize(list) + 11 + andData) * list->length * sizeof(char) + sizeof(char),
		cnt2 = (intsize(list->length) + intsize(SIZE_MAX) * 2 + hexsize(list) + 14 + andData) * sizeof(char);
	char* tmp = (char*) malloc(cnt2),
		*ret = (char*) malloc(cnt);

	memset(ret, 0, cnt);

	for (register size_t i = 0; i < list->length; i++) {
		snprintf(tmp, cnt2 - 1, "*%zu,%ld: %p (%zu) - %s", i, list->ids[i], list->data[i], list->lengths[i], list->data[i] != NULL ? hexify(list->data[i], list->lengths[i]) : "NULL");
		strcat(tmp, "\n");

		if (i) {
			strcat(ret, tmp);
		} else {
			strcpy(ret, tmp);
		}
	}

	free(tmp);

	return ret;
}

extern signed long scanindex(const void* ptr, const List* list) {
	register signed long out;

	for (out = 0; out < list->length; out++) {
		if (list->data[out] == ptr) return out;
	}

	return -1;
}

extern signed long customscan(bool (*scanindex)(const void * curptr, const void * tarptr), const void * ptr, const List * list) {
	register signed long out;

	for (out = 0; out < list->length; out++) {
		if (scanindex(list->data[out], ptr)) return out;
	}

	return -1;
}
