#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>

void* ec_malloc(size_t size);
void* ec_calloc(unsigned int count, size_t size);
char* ec_strdup(const char *str);
void* ec_realloc(void *ptr, size_t nsize);

#endif // __UTIL_H__
