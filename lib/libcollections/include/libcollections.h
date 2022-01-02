#ifndef __LIBCOLLECTIONS__
#define __LIBCOLLECTIONS__

#include <stdlib.h>
#include <stdbool.h>

typedef struct LinkedList {
    void* data;
    struct LinkedList* next;
} LinkedList;

LinkedList* linkedlist_new(void *data);
void linkedlist_add(LinkedList *head, void *data);
int linkedlist_len(LinkedList *head);
void linkedlist_free(LinkedList *head);

typedef struct Matrix {
    int **data;
    unsigned int width;
    unsigned int height;
} Matrix;

Matrix* matrix_new(unsigned int width, unsigned int height);
void matrix_copy(Matrix* src, Matrix* dest);
int matrix_sum(Matrix* m);
int matrix_neighbors8(Matrix *matrix, int *buf, unsigned int x, unsigned int y);
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

#endif // __LIBCOLLECTIONS__
