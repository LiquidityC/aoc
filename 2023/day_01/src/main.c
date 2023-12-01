#include <libinput.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static uint32_t digit_concat(char a, char b)
{
    char buf[3] = { '\0' };
    buf[0] = a;
    buf[1] = b;
    uint32_t sum = atoi(buf);
    return sum;
}

#define ONE "one", 3
#define TWO "two", 3
#define THREE "three", 5
#define FOUR "four", 4
#define FIVE "five", 4
#define SIX "six", 3
#define SEVEN "seven", 5
#define EIGHT "eight", 5
#define NINE "nine", 4

static bool read_digit(const char *c, char *val)
{
    if (isdigit(*c)) {
        *val = *c;
    } else if (strncmp(c, ONE) == 0) {
        *val = '1';
    } else if (strncmp(c, TWO) == 0) {
        *val = '2';
    } else if (strncmp(c, THREE) == 0) {
        *val = '3';
    } else if (strncmp(c, FOUR) == 0) {
        *val = '4';
    } else if (strncmp(c, FIVE) == 0) {
        *val = '5';
    } else if (strncmp(c, SIX) == 0) {
        *val = '6';
    } else if (strncmp(c, SEVEN) == 0) {
        *val = '7';
    } else if (strncmp(c, EIGHT) == 0) {
        *val = '8';
    } else if (strncmp(c, NINE) == 0) {
        *val = '9';
    } else {
        return false;
    }

    return true;
}

static void part1(Lines *lines)
{
    uint32_t sum = 0;
    for (size_t i = 0; i < lines->size; ++i) {
        char *c = lines->lines[i];
        char a, b;
        bool first = false;
        while (*c) {
            if (isdigit(*c)) {
                b = *c;
                if (!first) {
                    a = b;
                    first = true;
                }
            }
            c++;
        }
        sum += digit_concat(a, b);
    }

    printf("Part 1: %u\n", sum);
}

static void part2(Lines *lines)
{
    uint32_t sum = 0;
    for (size_t i = 0; i < lines->size; ++i) {
        char *c = lines->lines[i];
        char a, b;
        bool first = false;
        while (*c) {
            if (read_digit(c, &b)) {
                if (!first) {
                    a = b;
                    first = true;
                }
            }
            c++;
        }
        sum += digit_concat(a, b);
    }

    printf("Part 2: %u\n", sum);
}

int main(int argc, char **argv)
{
    int result = 0;
    Lines *lines = NULL;

    FILE *fp = fopen("input.txt", "r");
    if (fp != NULL) {
        lines = lines_read(fp);
        fclose(fp);
    } else {
        perror("fopen");
        result = -1;
        goto out;
    }

    part1(lines);
    part2(lines);

    lines_free(lines);
out:
    return result;
}
