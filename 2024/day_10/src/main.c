#include <assert.h>
#include <libinput.h>
#include <libcollections.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t height;
    bool visited;
} Area;

typedef struct {
    Area **area;
    uint8_t w;
    uint8_t h;
} Map;

static void reset_map(Map *map)
{
    for (size_t i = 0; i < map->h; ++i) {
        for (size_t j = 0; j < map->w; ++j) {
            map->area[i][j].visited = false;
        }
    }
}

static void parse_input(Map *map, Lines *lines)
{
    /* Allocate memory for the area */
    map->area = malloc(lines->size * sizeof(Area*));
    assert(map->area != NULL);
    for (size_t i = 0; i < lines->size; ++i) {
        map->area[i] = malloc(lines->length * sizeof(Area));
        assert(map->area[i] != NULL);
    }
    map->w = lines->length;
    map->h = lines->size;

    for (size_t i = 0; i < lines->size; ++i) {
        for (size_t j = 0; j < lines->length; ++j) {
            if (lines->lines[i][j] == '.') {
                map->area[i][j].height = 10;
            } else {
                map->area[i][j].height = lines->lines[i][j] - '0';
                map->area[i][j].visited = false;
            }
        }
    }
}

/**
 * Recursive Depth First Search to find trails
 */
static uint32_t find_trail_from(Map *map, int x, int y, uint8_t next_height, bool all_trails)
{
    /* Check so we are within the map */
    if (x < 0 || x >= map->w || y < 0 || y >= map->h) {
        return 0;
    }

    /* Check if we are att the expected height */
    uint8_t height = map->area[y][x].height;
    bool *visited = &map->area[y][x].visited;
    if (height != next_height) {
        return 0;
    }

    /* Check if we "peaked" */
    if (height == 9) {
        if(all_trails || !*visited) {
            *visited = true;
            return 1;
        } else {
            return 0;
        }
    }

    uint32_t count = 0;
    count += find_trail_from(map, x + 1, y, height + 1, all_trails);
    count += find_trail_from(map, x, y + 1, height + 1, all_trails);
    count += find_trail_from(map, x - 1, y, height + 1, all_trails);
    count += find_trail_from(map, x, y - 1, height + 1, all_trails);
    return count;
}

static uint32_t find_all_trails(Map *map, bool all_trails)
{
    uint32_t count = 0;
    for (size_t i = 0; i < map->h; ++i) {
        for (size_t j = 0; j < map->w; ++j) {
            if (map->area[i][j].height == 0) {
                count += find_trail_from(map, j, i, 0, all_trails);
                if (!all_trails) {
                    reset_map(map);
                }
            }
        }
    }
    return count;
}

static void part1(Map *map)
{
    printf("Part 1: %u\n", find_all_trails(map, false));
}
static void part2(Map *map)
{
    printf("Part 2: %u\n", find_all_trails(map, true));
}

int main(int argc, char **argv)
{
    Lines *lines;
    Map map;

    /* Read the input data */
    lines = lines_read_file("input.txt");
    parse_input(&map, lines);
    lines_free(lines);

    part1(&map);
    part2(&map);

    for (size_t i = 0; i < map.h; ++i) {
        free(map.area[i]);
    }
    free(map.area);

    return 0;
}
