#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "libcollections.h"

static void capcity_test(void)
{
    HashTable *ht = ht_create();

    assert(ht->capacity == 16);
    assert(ht->length == 0);

    int value = 42;
    char key[8];
    for (char c = 'a'; c != 'k'; ++c) {
        sprintf(key, "%c", c);
        ht_set(ht, key, &value);
    }

    printf("Capacity: %zu\n", ht->capacity);
    assert(ht->length == 10);
    assert(ht->capacity == 32);
}

static void get_test(void)
{
    HashTable *ht = ht_create();
    int value = 42;
    const char *key = ht_set(ht, "fortytwo", &value);
    assert(strcmp("fortytwo", key) == 0);
    int *result = ht_get(ht, "fortytwo");
    assert(42 == *result);
}

int main()
{
    capcity_test();
    get_test();
}

