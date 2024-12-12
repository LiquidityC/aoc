#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define HT_CAPACITY 40000

typedef uint64_t u64;

typedef struct ht_entry {
    u64 key;
    u64 val;
    u64 blink;
    struct ht_entry *next;
} Entry;

typedef struct hash_table {
    Entry *entries[HT_CAPACITY];
} HashTable;

static HashTable ht;

static void ht_insert(u64 key, u64 blink, u64 val)
{
    u64 index = (key * blink) % HT_CAPACITY;
    Entry *entry = ht.entries[index];
    Entry *new_entry = malloc(sizeof(Entry));
    new_entry->key = key;
    new_entry->val = val;
    new_entry->blink = blink;
    new_entry->next = NULL;

    if (entry == NULL) {
        ht.entries[index] = new_entry;
    } else {
        while (entry->next != NULL) {
            entry = entry->next;
        }
        entry->next = new_entry;
    }
}

static bool ht_find(u64 key, u64 blink, u64 *val)
{
    u64 index = (key * blink) % HT_CAPACITY;
    Entry *entry = ht.entries[index];
    while (entry != NULL) {
        if (entry->key == key && entry->blink == blink) {
            *val = entry->val;
            return true;
        }
        entry = entry->next;
    }
    return false;
}

static void ht_clear(void)
{
    for (size_t i = 0; i < HT_CAPACITY; ++i) {
        if (ht.entries[i] != NULL) {
            Entry *entry = ht.entries[i];
            Entry *next = NULL;
            while (entry != NULL) {
                next = entry->next;
                free(entry);
                entry = next;
            }
        }
    }
    memset(&ht, 0, sizeof(ht));
}

static size_t num_digits(u64 n)
{
    size_t count = 0;
    while (n != 0) {
        n /= 10;
        ++count;
    }
    return count;
}

/**
 * \brief Recursive function to count the evolution of one stone.
 *
 * Recursively counts the number of stones after a given number of blinks. Uses
 * a hash table for lookups to prevent doing the same calculations multiple
 * times.
 *
 * \param stone The current stone number.
 * \param blinks The number of blinks left.
 */
static u64 count_stones(u64 stone, size_t blinks)
{
    /* Check if we reached the end */
    if (blinks == 0) {
        return 1;
    }

    /* Check if we've done this before */
    u64 count = 0;
    if (ht_find(stone, blinks, &count)) {
        return count;
    }

    /* Recurse with rules */
    if (stone == 0) {
        count += count_stones(1, blinks - 1);
    } else {
        size_t digits = num_digits(stone);
        if (digits % 2 == 0) {
            size_t split = 1;
            for (size_t j = 0; j < digits / 2; ++j) {
                split *= 10;
            }
            u64 upper_half = stone / split;
            u64 lower_half = stone % split;
            count += count_stones(upper_half, blinks - 1);
            count += count_stones(lower_half, blinks - 1);
        } else {
            count += count_stones(stone * 2024, blinks - 1);
        }
    }

    /* Save the result */
    ht_insert(stone, blinks, count);
    return count;
}

static void part1(u64 *numbers, size_t count)
{
    u64 total = 0;
    for (size_t i = 0; i < count; ++i) {
        total += count_stones(numbers[i], 25);
    }
    printf("Part 1: %lu\n", total);
}

static void part2(u64 *numbers, size_t count)
{
    u64 total = 0;
    for (size_t i = 0; i < count; ++i) {
        total += count_stones(numbers[i], 75);
    }
    printf("Part 2: %lu\n", total);
}

int main(int argc, char **argv)
{
    u64 numbers[10];
    size_t count = 0;

    /* Read input */
    FILE *fp = fopen("input.txt", "r");
    assert(fp != NULL);
    while (!feof(fp)) {
        if (1 == fscanf(fp, "%lu", &numbers[count])) {
            count++;
        }
    }
    fclose(fp);

    /* Prep hash table */
    memset(&ht, 0, sizeof(ht));

    part1(numbers, count);
    part2(numbers, count);

    /* Free the hash table */
    ht_clear();

    return 0;
}
