#include <assert.h>
#include <libinput.h>
#include <libcollections.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define RULE_LENGTH 100
#define UPDATE_COUNT 250
#define UPDATE_LENGTH 50

static bool is_valid(uint32_t *update, bool (*rules)[RULE_LENGTH])
{
    size_t len = update[0];
    bool valid = true;
    for (size_t j = len; j > 0; --j) {
        uint32_t current = update[j];


        for (size_t k = 1; k < j; ++k) {

            /* If the current number has a rule against any preceeding number then
             * the update is bad */
            if (rules[current][update[k]]) {
                valid = false;
                break;
            }
        }
    }
    return valid;
}

static void fix_update(uint32_t *update, bool (*rules)[RULE_LENGTH])
{
    bool changed = true;
    size_t len = update[0];

    /* Bubble sort the update array */
    while (changed) {
        changed = false;
        for (size_t j = len; j > 0; --j) {
            uint32_t current = update[j];

            for (size_t k = 1; k < j; ++k) {
                if (rules[current][update[k]]) {
                    uint32_t tmp = update[j];
                    update[j] = update[k];
                    update[k] = tmp;
                    changed = true;
                    break;
                }
            }
        }
    }
}

static void part1(bool (*rules)[RULE_LENGTH], uint32_t (*updates)[UPDATE_LENGTH])
{
    uint32_t total = 0;
    for (size_t i = 0; i < UPDATE_COUNT; ++i) {
        if (updates[i][0] == 0) {
            break;
        }

        bool valid = is_valid(updates[i], rules);

        if (valid) {
            uint32_t mid = updates[i][(updates[i][0]+1)/2];
            total += mid;
        }
    }

    printf("Part 1: %u\n", total);
}

static void part2(bool (*rules)[RULE_LENGTH], uint32_t (*updates)[UPDATE_LENGTH])
{
    uint32_t total = 0;
    for (size_t i = 0; i < UPDATE_COUNT; ++i) {
        if (updates[i][0] == 0) {
            break;
        }

        bool valid = is_valid(updates[i], rules);
        if (!valid) {
            fix_update(updates[i], rules);
            uint32_t mid = updates[i][(updates[i][0]+1)/2];
            total += mid;
        }
    }

    printf("Part 2: %u\n", total);
}

int main(int argc, char **argv)
{
    Lines *lines;
    bool rules[100][100];
    uint32_t updates[250][50];
    size_t index;

    memset(rules, false, sizeof(rules));
    memset(updates, 0, sizeof(updates));

    lines = lines_read_file("input.txt");

    for (index = 0; index < lines->size; index++) {
        uint32_t a, b;
        if (lines->lines[index][0] == '\0') {
            break;
        }
        sscanf(lines->lines[index], "%u|%u", &a, &b);
        rules[a][b] = true;
    }

    size_t j = 0;
    for (; index < lines->size; index++) {
        size_t k = 1;
        uint32_t a;
        char *pos = lines->lines[index];

        while ((a = strtol(pos, &pos, 10)) != 0) {
            updates[j][k++] = a;
            if (*pos == '\0') {
                assert(k % 2 == 0);
                updates[j][0] = k - 1; // Put the update count in the first pos
                j++;
                break;
            }
            pos += 1; // Skip the ','
        }
    }

    lines_free(lines);

    part1(rules, updates);
    part2(rules, updates);

    return 0;
}
