#include <libcollections.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "util.h"

#define INITIAL_CAPACITY 16
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

static uint64_t hash_key(const char *key)
{
    uint64_t hash = FNV_OFFSET;
    for (const char *p = key; *p; ++p) {
        hash ^= (uint64_t)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

HashTable* ht_create(void)
{
    HashTable* ht = ec_malloc(sizeof(HashTable));
    ht->entries = ec_calloc(INITIAL_CAPACITY, sizeof(Entry));
    ht->capacity = INITIAL_CAPACITY;
    ht->length = 0;
    return ht;
}

void ht_destroy(HashTable* ht)
{
    for (size_t i = 0; i < ht->capacity; ++i) {
        if (ht->entries[i].key)
            free((void*) ht->entries[i].key);
    }
    free(ht->entries);
    free(ht);
}

void* ht_get(HashTable* ht, const char *key)
{
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(ht->capacity - 1));

    while (ht->entries[index].key != NULL) {
        if (strcmp(key, ht->entries[index].key) == 0) {
            return ht->entries[index].value;
        }
        index = (index + 1) % ht->capacity;
    }
    return NULL;
}

static const char* ht_set_entry(Entry *entries,
                                size_t capacity,
                                const char *key,
                                void *value,
                                size_t* plen)
{
    uint64_t hash = hash_key(key);
    size_t index = (size_t)(hash & (uint64_t)(capacity - 1));

    while (entries[index].key != NULL) {
        if (strcmp(key, entries[index].key) == 0) {
            entries[index].value = value;
            return entries[index].key;
        }

        index = (index + 1) % capacity;
    }

    if (plen != NULL) {
        key = ec_strdup(key);
        (*plen)++;
    }
    entries[index].key = (char*) key;
    entries[index].value = value;
    return entries[index].key;
}

static bool ht_expand(HashTable* ht)
{
    size_t new_cap = ht->capacity * 2;
    if (new_cap < ht->capacity)
        return false; // Overflow

    Entry* entries = ec_calloc(new_cap, sizeof(Entry));
    for (size_t i = 0; i < ht->capacity; ++i) {
        Entry* e = &ht->entries[i];
        if (e->key != NULL) {
            ht_set_entry(entries, new_cap, e->key, e->value, NULL);
        }
    }
    free(ht->entries);
    ht->entries = entries;
    ht->capacity = new_cap;
    return true;
}

const char* ht_set(HashTable* ht, const char *key, void *value)
{
    if (value == NULL) {
        return NULL;
    }

    if (ht->length >= (ht->capacity/2)) {
        if (!ht_expand(ht))
            return NULL;
    }

    return ht_set_entry(ht->entries, ht->capacity, key, value, &ht->length);
}

size_t ht_length(HashTable *ht)
{
    return ht->length;
}

HashTableIter ht_iterator(HashTable *ht)
{
    HashTableIter hti;
    hti._ht = ht;
    hti._index = 0;
    return hti;
}

bool ht_next(HashTableIter *hti)
{
    HashTable *ht = hti->_ht;
    while (hti->_index < ht->capacity) {
        size_t i = hti->_index;
        hti->_index++;
        if (ht->entries[i].key != NULL) {
            Entry e = ht->entries[i];
            hti->key = e.key;
            hti->value = e.value;
            return true;
        }
    }
    return false;
}
