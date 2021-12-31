#include "util.h"
#include <stdio.h>

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
