#include <stdlib.h>
#include <stdio.h>
#include <libinput.h>
#include <assert.h>
#include <string.h>
#include <libcollections.h>
#include <limits.h>

typedef struct {
    char key[32];
    char value[32];
} Repl;

static Repl repl_from(const char *line)
{
    Repl repl;
    sscanf(line,
           "%s => %s",
           repl.key,
           repl.value
           );

    return repl;
}

static void strinsert(char *buf, const char *a, const char *b, size_t start, size_t len)
{
    strncpy(buf, a, start);
    strcat(buf, b);
    if (strlen(a) >= start + len)
        strcat(buf, a + start + len);
}

static HashTable* replace(const char *src, Repl *replacements, size_t repl_count, size_t val)
{
    HashTable *ht = ht_create();
    size_t src_len = strlen(src);
    for (size_t i = 0; i < src_len; ++i) {
        for (size_t j = 0; j < repl_count; ++j) {
            size_t klen = strlen(replacements[j].key);
            if (strncmp(src + i, replacements[j].key, klen) == 0) {
                char buffer[1024] = { '\0' };
                strinsert(buffer, src, replacements[j].value, i, klen);
                size_t *count = malloc(sizeof(size_t));
                *count = val;
                size_t *existing = ht_get(ht, buffer);
                if (existing != NULL)
                    free(existing);
                ht_set(ht, buffer, count);
            }
        }
    }

    return ht;
}

typedef struct {
    char *str;
    size_t count;
} Pair;

static size_t deconstruct(const char *src, const Repl *replacements, size_t repl_count, size_t count, HashTable *cache)
{
    if (strcmp(src, "e") == 0) {
        size_t *score = malloc(sizeof(size_t));
        *score = count - 1;
        ht_set(cache, src, score);
        return *score;
    }

    size_t *goal = ht_get(cache, "e");
    if (goal != NULL) {
        return *goal;
    }

    size_t *cache_val = ht_get(cache, src);
    if (cache_val != NULL) {
        return *cache_val;
    }

    size_t result = INT_MAX;
    for (size_t j = 0; j < repl_count; ++j) {
        if (ht_get(cache, "goal") != NULL) {
            result = *((size_t*) ht_get(cache, "goal"));
            break;
        }
        size_t vlen = strlen(replacements[j].value);
        char *pos = strstr(src, replacements[j].value);
        if (pos != NULL) {
            char buffer[512] = { '\0' };
            strinsert(buffer, src, replacements[j].key, pos - src, vlen);
            size_t response = deconstruct(buffer, replacements, repl_count, count + 1, cache);

            size_t *score = ht_get(cache, src);
            if (score == NULL) {
                size_t *score = malloc(sizeof(size_t));
                *score = response;
                ht_set(cache, src, score);
            } else {
                *score = *score < response ? *score : response;
            }
            result = result < response ? result : response;
        }
    }
    return result;
}

static int repl_cmp(const void *a, const void *b)
{
    Repl *r1 = (Repl*) a;
    Repl *r2 = (Repl*) b;
    return strlen(r2->value)-strlen(r1->value);
}

int main()
{
    FILE *fp = fopen("input.txt", "r");
    Lines *lines = lines_read(fp);
    int repl_count = lines->size-2;

    Repl *replacements = calloc(repl_count, sizeof(Repl));
    char source[1024];
    assert(replacements != NULL);

    for (int i = 0; i < lines->size-2; ++i) {
        replacements[i] = repl_from(lines->lines[i]);
    }
    qsort(replacements, repl_count, sizeof(Repl), repl_cmp);
    strcpy(source, lines->lines[lines->size-1]);
    lines_free(lines);

    HashTable *results = replace(source, replacements, repl_count, 1);
    printf("Part 1: %zu\n", ht_length(results));
    HashTableIter it = ht_iterator(results);
    while (ht_next(&it))
        free((size_t*) it.value);
    ht_destroy(results);

    HashTable *cache = ht_create();
    size_t response = deconstruct(source, replacements, repl_count, 1, cache);
    printf("Part 2: %zu\n", response);

    it = ht_iterator(cache);
    while (ht_next(&it))
        free((size_t*) it.value);
    ht_destroy(cache);
    free(replacements);
    return 0;
}
