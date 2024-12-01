#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libcollections.h"
#include "util.h"

void array_init(Array *a, size_t capacity)
{
	a->array = ec_calloc(capacity, sizeof(int64_t));
	a->size = capacity;
	a->used = 0;
}

void array_add(Array *a, int64_t v)
{
	a->array[a->used++] = v;
	if (a->used == a->size) {
		a->size *= 2;
		a->array = ec_realloc(a->array, a->size * sizeof(int64_t));
	}
}

void array_del(Array *a, size_t index)
{
	a->used--;
	for (size_t i = index; i < a->used; ++i) {
		a->array[i] = a->array[i+1];
	}
}

void array_insert(Array *a, size_t index, int64_t v)
{
	if (index > a->used) {
		array_add(a, v);
	} else {
		a->used++;
		if (a->used == a->size) {
			a->size *= 2;
			a->array = ec_realloc(a->array, a->size * sizeof(int64_t));
		}
		for (size_t i = a->used-1; i > index; --i) {
			a->array[i] = a->array[i-1];
		}
		a->array[index] = v;
	}
}

void array_print(Array *a)
{
	for (size_t i = 0; i < a->used; ++i) {
		printf("%ld ", a->array[i]);
	}
	printf("\n");
}

void array_copy(Array *dst, const Array *src)
{
    dst->array = ec_realloc(dst->array, src->size * sizeof(int64_t));
    dst->size = src->size;
    dst->used = src->used;
    memcpy(dst->array, src->array, src->used * sizeof(int64_t));
}

void array_sort(Array *a, int (*cmp)(const void *, const void *))
{
    qsort(a->array, a->used, sizeof(int64_t), cmp);
}

void array_free(Array *a)
{
	if (a->array != NULL)
		free(a->array);
	a->array = NULL;
}
