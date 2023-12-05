
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <libinput.h>
#include <libcollections.h>

typedef uint64_t u64;

typedef struct {
    u64 dstart;
    u64 sstart;
    size_t len;
} Range;

typedef struct {
    Range ranges[50];
    u64 begin, end;
    size_t count;
} Map;

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

static size_t read_seeds(const char *line, u64 *seeds)
{
    char *start = strchr(line, ':');
    start += 2;

    size_t i = 0;
    u64 num = strtol(start, &start, 10);
    while (num) {
        seeds[i++] = num;
        num = strtol(start, &start, 10);
    }
    return i;
}

static size_t read_map(Lines *lines, size_t *index, const char *label, Map *map)
{
    size_t i = *index;

    /* Confirm the label */
    assert(strncmp(lines->lines[i], label, strlen(label)) == 0);
    i++;

    /* Read the numbers */
    size_t ri = 0;
    while (i < lines->size && strlen(lines->lines[i]) > 0) {
        char *line = lines->lines[i++];
        char *p = line;
        Range *r = &map->ranges[ri++];
        r->dstart = strtol(p, &p, 10);
        assert(r->dstart >= 0);
        r->sstart = strtol(p, &p, 10);
        assert(r->sstart >= 0);
        r->len = strtol(p, &p, 10);
        assert(r->len >= 0);
    }

    *index = i + 1;
    map->count = ri;

    return ri;
}

#define MAP_COUNT 7

static u64 trace_seed(u64 start, Map *maps)
{
    u64 next = start;

    for (size_t i = 0; i < MAP_COUNT; ++i) {
        Map *map = &maps[i];
        for (size_t j = 0; j < map->count; ++j) {
            Range *r = &map->ranges[j];
            if (next >= r->sstart && next < r->sstart + r->len) {
                u64 diff = next - r->sstart;
                next = r->dstart + diff;
                break;
            }
        }
    }
    return next;
}

static u64 trace_range(u64 start, u64 end, size_t map_index, size_t range_index, Map *maps)
{
    if (map_index >= MAP_COUNT) {
        return start;
    }

    Map *map = &maps[map_index];
    u64 min = 0u - 1;
    for (size_t i = range_index; i < map->count; ++i) {
        Range *r = &map->ranges[i];
        u64 rend = r->sstart + r->len;
        if (start < r->sstart && end >= rend) {
            min = MIN(min, trace_range(start, r->sstart - 1, map_index, i + 1, maps));
            min = MIN(min, trace_range(r->dstart, r->dstart + r->len - 1, map_index + 1, 0, maps));
            min = MIN(min, trace_range(r->sstart + r->len, end, map_index, i + 1, maps));
        } else if (start < r->sstart && end > r->sstart) {
            u64 diff = end - r->sstart;
            end = r->sstart - 1;
            min = MIN(min, trace_range(r->dstart, r->dstart + diff, map_index + 1, 0, maps));
        } else if (start >= r->sstart && start < rend && end >= rend) {
            u64 diff = start - r->sstart;
            start = r->sstart + r->len;
            min = MIN(min, trace_range(r->dstart + diff, r->dstart + r->len - 1, map_index + 1, 0, maps));
        } else if (start >= r->sstart && end < rend) {
            u64 sdiff = start - r->sstart;
            u64 ediff = end - r->sstart;
            return MIN(min, trace_range(r->dstart + sdiff, r->dstart + ediff, map_index + 1, 0, maps));
        }
    }
    return MIN(min, trace_range(start, end, map_index + 1, 0, maps));
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

    size_t seed_len;
    u64 seeds[20];
    seed_len = read_seeds(lines->lines[0], seeds);
    assert(seed_len > 0 && seed_len <= 20 && seed_len % 2 == 0);

    Map maps[MAP_COUNT] = {0};

    size_t mi = 0;
    size_t line_index = 2;
    read_map(lines, &line_index, "seed-to-soil map", &maps[mi++]);
    read_map(lines, &line_index, "soil-to-fertilizer map", &maps[mi++]);
    read_map(lines, &line_index, "fertilizer-to-water map", &maps[mi++]);
    read_map(lines, &line_index, "water-to-light map", &maps[mi++]);
    read_map(lines, &line_index, "light-to-temperature map", &maps[mi++]);
    read_map(lines, &line_index, "temperature-to-humidity map", &maps[mi++]);
    read_map(lines, &line_index, "humidity-to-location map", &maps[mi++]);

    //check_map_ranges(maps);

    u64 nearest = 0u - 1;
    for (size_t i = 0; i < seed_len; ++i) {
        u64 location = trace_seed(seeds[i], maps);
        nearest = MIN(location, nearest);
    }
    printf("Part 1: %lu\n", nearest);

    nearest = 0u - 1;
    for (size_t i = 0; i < seed_len; i += 2) {
        u64 location = trace_range(seeds[i], seeds[i] + seeds[i+1] - 1, 0, 0, maps);
        nearest = MIN(location, nearest);
    }
    printf("Part 2: %lu\n", nearest);

out:
    if (lines) {
        lines_free(lines);
    }
    return result;
}
