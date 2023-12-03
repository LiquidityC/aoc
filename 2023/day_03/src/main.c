#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <libinput.h>
#include <libcollections.h>


typedef struct {
    size_t x, y;
    uint64_t ratio;
    size_t count;
} Gear;

static Gear* gear_create(void)
{
    Gear *gear = malloc(sizeof(Gear));
    if (!gear) {
        perror("malloc");
        return NULL;
    }
    memset(gear, 0, sizeof(Gear));
    return gear;
}

static void gear_get_key(Gear *gear, char *buf)
{
    sprintf(buf, "%lux%lu", gear->x, gear->y);
}

static Gear* get_gear(Lines *lines, size_t row, size_t s, size_t e)
{
    size_t start = s > 0 ? s - 1 : 0;
    size_t end = e < lines->length - 1 ? e + 1 : e;
    char *r;

    Gear *gear = gear_create();

    // Check row above
    if (row > 0) {
        r = lines->lines[row-1];
        for (size_t i = start; i <= end; ++i) {
            if (r[i] == '*') {
                gear->x = i;
                gear->y = row-1;
                return gear;
            }
        }
    }

    // Check same row
    r = lines->lines[row];
    if (r[start] == '*') {
        gear->x = start;
        gear->y = row;
        return gear;
    } else if (r[end] == '*') {
        gear->x = end;
        gear->y = row;
        return gear;
    }

    // Check row below
    if (row < lines->size - 1) {
        r = lines->lines[row+1];
        for (size_t i = start; i <= end; ++i) {
            if (r[i] == '*') {
                gear->x = i;
                gear->y = row+1;
                return gear;
            }
        }
    }

    free(gear);

    return NULL;
}

static bool is_partnum(Lines *lines, size_t row, size_t s, size_t e)
{
    size_t start = s > 0 ? s - 1 : 0;
    size_t end = e < lines->length - 1 ? e + 1 : e;
    char *r;

    // Check row above
    if (row > 0) {
        r = lines->lines[row-1];
        for (size_t i = start; i <= end; ++i) {
            if (r[i] != '.' && !isdigit(r[i])) {
                return true;
            }
        }
    }

    // Check same row
    r = lines->lines[row];
    if (r[start] != '.' && !isdigit(r[start])) {
        return true;
    } else if (r[end] != '.' && !isdigit(r[end])) {
        return true;
    }

    // Check row below
    if (row < lines->size - 1) {
        r = lines->lines[row+1];
        for (size_t i = start; i <= end; ++i) {
            if (r[i] != '.' && !isdigit(r[i])) {
                return true;
            }
        }
    }

    return false;
}

static void add_gear(HashTable *gears, Gear *gear, uint64_t pn)
{
    if (gear == NULL) {
        return;
    }

    char key[10];
    gear_get_key(gear, key);
    Gear *existing = ht_get(gears, key);
    if (existing) {
        if (existing->count == 2)
            printf("%lu * %lu = %lu\n", existing->ratio, pn, existing->ratio * pn);
        existing->ratio *= pn;
        existing->count++;
        free(gear);
    } else {
        gear->ratio = pn;
        gear->count = 1;
        ht_set(gears, key, gear);
    }
}

static void parse_numbers(Lines *lines)
{
    HashTable *gears = ht_create();
    uint64_t sum = 0;

    for (size_t y = 0; y < lines->size; ++y) {

        bool digit = false;
        size_t s, e;
        for (size_t x = 0; x < lines->length; ++x) {
            char c = lines->lines[y][x];
            if (isdigit(c) && !digit) {
                s = x;
                digit = true;
            } else if (digit && !isdigit(c)) {
                e = x-1;
                uint64_t pn = strtol(lines->lines[y] + s, NULL, 10);
                if (is_partnum(lines, y, s, e)) {
                    sum += pn;
                    add_gear(gears, get_gear(lines, y, s, e), pn);
                }
                digit = false;
            }
        }
        if (digit) {
            if (is_partnum(lines, y, s, lines->length - 1)) {
                uint64_t pn = strtol(lines->lines[y] + s, NULL, 10);
                sum += pn;
                add_gear(gears, get_gear(lines, y, s, lines->length - 1), pn);
            }
        }
    }

    printf("Part 1: %lu\n", sum);

    // Add the gears together
    uint64_t ratios = 0;
    HashTableIter it = ht_iterator(gears);
    while (ht_next(&it)) {
        Gear *g = (Gear*) it.value;
        if (g->count == 2) {
            ratios += g->ratio;
        }
        free(g);
    }
    ht_destroy(gears);
    printf("Part 2: %lu\n", ratios);
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    int result = 0;

    Lines *lines = lines_read_file("input.txt");
    if (!lines) {
        perror("lines_read_file");
        result = -1;
        goto out;
    }

    parse_numbers(lines);
out:
    if (lines) {
        lines_free(lines);
    }
    return result;
}
