#include "libcollections.h"
#include <assert.h>

static void swap(int64_t *a, int64_t *b)
{
	int64_t tmp = *a;
	*a = *b;
	*b = tmp;
}

static void heapify(Array *a, int i)
{
	if (a->used > 1) {
		size_t size = a->used;
		size_t largest = i;
		size_t l = 2 * i + 1;
		size_t r = 2 * i + 2;
		if (l < size && a->array[l] > a->array[largest])
			largest = l;
		if (r < size && a->array[r] > a->array[largest])
			largest = r;
		if ((int) largest != i)
		{
			swap(&a->array[i], &a->array[largest]);
			heapify(a, largest);
		}
	}
}

void heap_init(Array *a)
{
	int size = (int) a->used;
	for (int i = size / 2 - 1; i >= 0; --i) {
		heapify(a, i);
	}
}

void heap_insert(Array *a, int64_t v)
{
	array_add(a, v);
	heap_init(a);
}

void heap_delete(Array *a, int64_t v)
{
	size_t i;
	for (i = 0; i < a->used; ++i) {
		if (a->array[i] == v)
			break;
	}
	a->used--;
	swap(&a->array[i], &a->array[a->used]);
	heap_init(a);
}

int64_t heap_peek(Array *a)
{
	assert(a->used > 0);
	return a->array[0];
}
