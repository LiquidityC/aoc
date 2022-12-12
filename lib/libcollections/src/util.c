#include <stdio.h>
#include <string.h>
#include "util.h"

void* ec_malloc(size_t size)
{
    void *m = malloc(size);
    if (m == NULL) {
        fprintf(stderr, "[!!] malloc(%zu) failed\n", size);
        exit(-1);
    }
    return m;
}

void* ec_calloc(unsigned int count, size_t size)
{
    void *m = calloc(count, size);
    if (m == NULL) {
        fprintf(stderr, "[!!] calloc(%d, %zu) failed\n", count, size);
        exit(-1);
    }
    return m;
}

char* ec_strdup(const char *str)
{
    char *new = strdup(str);
    if (new == NULL) {
        fprintf(stderr, "[!!] strdup(%s) failed\n", str);
        exit(-1);
    }
    return new;
}

void* ec_realloc(void *ptr, size_t nsize)
{
	void *nptr = realloc(ptr, nsize);
	if (nptr == NULL) {
		fprintf(stderr, "[!!] realloc(%p, %lu) failed\n", ptr, nsize);
		exit(-1);
	}
	return nptr;
}
