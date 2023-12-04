
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <libinput.h>

static uint32_t parse_card_matches(char *line)
{
    char *substr = strchr(line, ':');
    if (substr == NULL) {
        perror("strchr");
        return 0;
    }
    substr += 2;

    uint32_t winners[100] = {0};

    char *token;
    char *start;

    token = strtok(substr, "|");
    /* Parse winning numbers */
    start = token;
    while (*start) {
        size_t num = strtol(start, &start, 10);
        if (num > 0) {
            winners[num] = 1;
        } else {
            start++;
        }
    }

    /* Parse the ticket numbers */
    uint32_t matches = 0;
    token = strtok(NULL, "|");
    start = token;
    while (*start != '\0') {
        size_t num = strtol(start, &start, 10);
        if (num > 0) {
            if (winners[num] > 0) {
                matches += 1;
            }
        } else {
            start++;
        }
    }

    return matches;
}

static void parse_cards(Lines *lines)
{
    uint64_t dup_table[300];
    for (size_t i = 0; i < 300; ++i) {
        dup_table[i] = 1;
    }

    uint32_t score = 0;
    for (size_t i = 0; i < lines->size; ++i) {
        uint32_t matches = parse_card_matches(lines->lines[i]);
        if (matches > 0) {
            score += pow(2, matches-1);
        }
        for (size_t j = 0; j < matches; ++j) {
            dup_table[i + j + 1] += dup_table[i];
        }
    }
    printf("Part 1: %u\n", score);

    uint64_t total_cards = 0;
    for (size_t i = 0; i < lines->size; ++i) {
        total_cards += dup_table[i];
    }
    printf("Part 2: %lu\n", total_cards);
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

    parse_cards(lines);

out:
    if (lines) {
        lines_free(lines);
    }
    return result;
}
