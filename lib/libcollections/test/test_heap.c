#include <libcollections.h>
#include <assert.h>
#include <stdio.h>

static void test_init(void)
{
	Array a;
	array_init(&a, 10);
	array_add(&a, 3);
	array_add(&a, 5);
	array_add(&a, 1);
	array_add(&a, 7);
	array_add(&a, 9);
	array_add(&a, 2);

	heap_init(&a);
	assert(a.array[0] == 9);
}

static void test_insert(void)
{
	Array a;
	array_init(&a, 10);
	heap_init(&a);
	heap_insert(&a, 3);
	heap_insert(&a, 5);
	heap_insert(&a, 1);
	heap_insert(&a, 7);
	heap_insert(&a, 9);
	heap_insert(&a, 2);
	assert(a.array[0] == 9);
}

static void test_delete(void)
{
	Array a;
	array_init(&a, 10);
	heap_init(&a);
	heap_insert(&a, 3);
	heap_insert(&a, 5);
	heap_insert(&a, 1);
	heap_insert(&a, 7);
	heap_insert(&a, 9);
	heap_insert(&a, 2);
	assert(a.array[0] == 9);
	heap_delete(&a, 9);
	assert(a.array[0] == 7);
	heap_delete(&a, 7);
	assert(a.array[0] == 5);
}

static void test_big_insert(void)
{
	Array a;
	array_init(&a, 10);

	for (int64_t i = 0; i < 100; ++i) {
		heap_insert(&a, i);
	}
	assert(a.array[0] == 99);
	heap_delete(&a, 99);
	assert(a.array[0] == 98);
}

int main(void)
{
	test_init();
	test_insert();
	test_delete();
	test_big_insert();
}
