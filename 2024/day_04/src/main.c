#include <assert.h>
#include <libinput.h>
#include <libcollections.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    int x;
    int y;
} Vec2;

static bool find_xmas(Lines *lines, Vec2 start, Vec2 dir) {
    static const char *word = "XMAS";

    Vec2 pos = start;
    for (size_t i = 0; i < strlen(word); i++) {
        char chr = word[i];
        if (chr != lines->lines[pos.y][pos.x]) {
            return false;
        }
        if (chr == 'S') {
            /* We matched the entire word */
            return true;
        }
        pos.x += dir.x;
        pos.y += dir.y;
        if (pos.x < 0 || pos.x >= lines->length || pos.y < 0 || pos.y >= lines->size) {
            return false;
        }
    }
    return true;
}

static void part1(Lines *lines)
{
    static const Vec2 dirs[] = {
        {0, 1},
        {1, 0},
        {1, 1},
        {1, -1},
        {-1, 1},
        {-1, -1},
        {0, -1},
        {-1, 0},
    };

    uint32_t total = 0;

    for (int i = 0; i < lines->size; i++) {
        for (int j = 0; j < lines->size; j++) {
            for (int k = 0; k < 8; k++) {
                Vec2 start = {j, i};
                Vec2 dir = dirs[k];
                if (find_xmas(lines, start, dir)) {
                    total++;
                }
            }
        }
    }

    printf("Part 1: %d\n", total);
}

static bool find_cross_mas(Lines *lines, Vec2 origin)
{
    char chr = lines->lines[origin.y][origin.x];

    if (chr != 'A') {
        return false;
    }

    if (origin.x == 0 || origin.x == lines->length - 1 || origin.y == 0 ||
        origin.y == lines->size - 1)
    {
        return false;
    }

    char top_left = lines->lines[origin.y - 1][origin.x - 1];
    char bottom_right = lines->lines[origin.y + 1][origin.x + 1];

    if (top_left != 'M' && top_left != 'S') {
        return false;
    } else if (top_left == 'M' && bottom_right != 'S') {
        return false;
    } else if (top_left == 'S' && bottom_right != 'M') {
        return false;
    }

    char top_right = lines->lines[origin.y - 1][origin.x + 1];
    char bottom_left = lines->lines[origin.y + 1][origin.x - 1];

    if (top_right != 'M' && top_right != 'S') {
        return false;
    } else if (top_right == 'M' && bottom_left != 'S') {
        return false;
    } else if (top_right == 'S' && bottom_left != 'M') {
        return false;
    }

    return true;
}

static void part2(Lines *lines)
{
    uint32_t total = 0;
    for (int i = 0; i < lines->size; i++) {
        for (int j = 0; j < lines->length; j++) {
            Vec2 origin = {j, i};
            if (find_cross_mas(lines, origin)) {
                total += 1;
            }
        }
    }
    printf("Part 2: %d\n", total);
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
