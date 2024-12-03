#include <assert.h>
#include <libinput.h>
#include <libcollections.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static void part1(Lines *lines)
{
    uint32_t sum = 0;

    for (size_t i = 0; i < lines->size; i++) {
        char *line = lines->lines[i];
        uint32_t a, b;
        char *pos = line;

        while (true) {
            pos = strstr(pos, "mul(");
            if (pos == NULL) {
                break;
            }

            /* Skip the "mul(" */
            pos += 4;

            /* Rad first number */
            a = strtol(pos, &pos, 10);
            if (*pos != ',') {
                continue;
            }

            /* Skip the "," */
            pos += 1;

            /* Read second number */
            b = strtol(pos, &pos, 10);

            /* Confirm the closing parenthesis */
            if (*pos != ')') {
                continue;
            }

            sum += a * b;
        }
    }

    printf("Part 1: %u\n", sum);
}

static void part2(Lines *lines)
{
    uint32_t sum = 0;
    bool do_mul = true;

    for (size_t i = 0; i < lines->size; ++i) {
        char *pos = lines->lines[i];

        uint32_t a, b;
        while (*pos != '\0') {
            if (do_mul && strncmp(pos, "mul(", 4) == 0) {
                pos += 4;
                a = strtol(pos, &pos, 10);
                if (*pos != ',') {
                    continue;
                }
                pos += 1;
                b = strtol(pos, &pos, 10);
                if (*pos != ')') {
                    continue;
                }
                sum += a * b;
                pos += 1;
            } else if (strncmp(pos, "do()", 3) == 0) {
                do_mul = true;
                pos += 4;
            } else if (strncmp(pos, "don't()", 7) == 0) {
                do_mul = false;
                pos += 7;
            } else {
                pos += 1;
            }
        }
    }

    printf("Part 2: %u\n", sum);
}

int main(int argc, char **argv)
{
    Lines *lines = NULL;

    lines = lines_read_file("input.txt");

    part1(lines);
    part2(lines);

    lines_free(lines);

    return 0;
}
