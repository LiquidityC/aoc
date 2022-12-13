#include <stdlib.h>
#include <assert.h>
#include <libcollections.h>

static void test_init(void)
{
	Array a;
	array_init(&a, 5);
	assert(a.size == 5);
	assert(a.used == 0);
	for (size_t i = 0; i < a.size; ++i) {
		assert(a.array[i] == 0);
	}

	array_free(&a);
}

static void test_add(void)
{
	Array a;
	array_init(&a, 2);

	array_add(&a, 0);
	assert(a.size == 2);
	assert(a.used == 1);
	assert(a.array[0] == 0);

	array_add(&a, 1);
	assert(a.size == 4);
	assert(a.used == 2);
	assert(a.array[1] == 1);

	array_free(&a);
}

static void test_del(void)
{
	Array a;
	array_init(&a, 5);

	int64_t i = 0;
	array_add(&a, i++);
	array_add(&a, i++);
	array_add(&a, i++);
	array_add(&a, i++);

	assert(a.array[1] == 1);
	assert(a.array[2] == 2);
	assert(a.array[3] == 3);
	assert(a.used == 4);
	assert(a.size == 5);
	array_del(&a, 1);
	assert(a.array[1] == 2);
	assert(a.array[2] == 3);
	assert(a.used == 3);
	assert(a.size == 5);

	array_free(&a);
}

static void test_insert(void)
{
	Array a;
	array_init(&a, 5);

	int64_t i = 0;
	array_add(&a, i++);
	array_add(&a, i++);
	array_add(&a, i++);
	array_add(&a, i++);

	assert(a.used == 4);
	assert(a.size == 5);

	array_insert(&a, 1, 10);

	assert(a.array[0] == 0);
	assert(a.array[1] == 10);
	assert(a.array[2] == 1);
	assert(a.array[3] == 2);
	assert(a.array[4] == 3);
	assert(a.used == 5);
	assert(a.size == 10);

	array_free(&a);
}

int main(void)
{
	test_init();
	test_add();
	test_del();
	test_insert();
}
