#ifndef __LIBCOLLECTIONS__
#define __LIBCOLLECTIONS__

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct LinkedList {
    void* data;					/**< The node data */
    struct LinkedList* next;
} LinkedList;

LinkedList* linkedlist_new(void *data);
void linkedlist_add(LinkedList *head, void *data);
int linkedlist_len(LinkedList *head);
void linkedlist_free(LinkedList *head);

typedef struct Matrix {
    int64_t **data;
    size_t width;
    size_t height;
} Matrix;

Matrix* matrix_new(size_t width, size_t height);
void matrix_copy(Matrix* src, Matrix* dest);
int64_t matrix_sum(Matrix* m);
int64_t matrix_neighbors4(Matrix *matrix, int64_t *buf, size_t x, size_t y);
int64_t matrix_neighbors8(Matrix *matrix, int64_t *buf, size_t x, size_t y);
bool matrix_contains_point(Matrix *matrix, int64_t x, int64_t y);
void matrix_print(Matrix* m);
void matrix_free(Matrix* matrix);

typedef struct Entry {
    char *key;
    void *value;
} Entry;

typedef struct HashTable {
    Entry *entries;
    size_t length;
    size_t capacity;
} HashTable;

typedef struct HashTableIter {
    const char* key;
    const void* value;
    HashTable *_ht;
    size_t _index;
} HashTableIter;

HashTable* ht_create(void);
void* ht_get(HashTable *table, const char *key);
void ht_destroy(HashTable *table);
const char* ht_set(HashTable *table, const char *key, void *value);
void* ht_remove(HashTable *table, const char *key);
void ht_clear(HashTable *table);
size_t ht_length(HashTable *table);
HashTableIter ht_iterator(HashTable *table);
bool ht_next(HashTableIter *iter);

typedef struct Array {
	int64_t *array;
	size_t used;
	size_t size;
} Array;

void array_init(Array *a, size_t capacity);
void array_add(Array *a, int64_t v);
void array_del(Array *a, size_t index);
void array_insert(Array *a, size_t index, int64_t v);
void array_print(Array *a);
void array_copy(Array *dst, const Array *src);
void array_sort(Array *a, int (*cmp)(const void *, const void *));
void array_free(Array *a);

void heap_init(Array *a);
void heap_insert(Array *a, int64_t v);
void heap_delete(Array *a, int64_t v);
int64_t heap_peek(Array *a);


#endif // __LIBCOLLECTIONS__
