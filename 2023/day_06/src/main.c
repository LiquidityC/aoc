

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <libinput.h>
#include <libcollections.h>

typedef uint32_t u32;
typedef uint64_t u64;

#define BUFLEN 10

static size_t read_line(const char *line, u32 *buf)
{
    char *start = strchr(line, ':') + 1;

    size_t count = 0;
    u32 num = strtol(start, &start, 10);
    while (num) {
        buf[count++] = num;
        num = strtol(start, &start, 10);
    }

    return count;
}

static size_t read_input(Lines *lines, u32 *times, u32 *distances)
{
    size_t t_count = read_line(lines->lines[0], times);
    size_t d_count = read_line(lines->lines[1], distances);
    assert (t_count == d_count);
    return t_count;
}

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

static u64 num_concat(u64 a, u64 b)
{
    u64 pow = 10;
    while (b >= pow) {
        pow *= 10;
    }
    return a * pow + b;
}

static u64 count_wins(u64 t, u64 d)
{
    u64 wins = 0;
    for (u64 hold = 0; hold < t; ++hold) {
        if (hold * (t-hold) > d) {
            wins++;
        }
    }
    return wins;
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    u32 times[BUFLEN];
    u32 distances[BUFLEN];

    int result = 0;

    Lines *lines = lines_read_file("input.txt");
    if (!lines) {
        perror("lines_read_file");
        result = -1;
        goto out;
    }

    size_t len = read_input(lines, times, distances);

    u32 total = 1;
    u64 long_t = 0;
    u64 long_d = 0;
    for (size_t i = 0; i < len; ++i) {
        u32 t = times[i];
        u32 d = distances[i];
        total *= count_wins(t, d);

        if (long_t == 0) {
            long_t = t;
            long_d = d;
        } else {
            long_t = num_concat(long_t, t);
            long_d = num_concat(long_d, d);
        }
    }

    printf("Part 1: %u\n", total);
    printf("Part 2: %lu\n", count_wins(long_t, long_d));

out:
    if (lines) {
        lines_free(lines);
    }
    return result;
}
