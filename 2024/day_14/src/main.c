#include <libinput.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROBOTS 501

typedef struct vec_2 {
    int32_t x;
    int32_t y;
} Vec2;

typedef struct robot {
    Vec2 pos;
    Vec2 dir;
} Robot;

static uint32_t simulate_movement(Robot *robots, size_t len, int steps, size_t w, size_t h)
{
    Robot *r;
    Vec2 pos;

    size_t wmid = w / 2;
    size_t hmid = h / 2;

    /* Quadrants:
     * | 0 | 1 |
     * | 2 | 3 |
     */
    uint32_t quadrants[4] = {0};

    for (size_t i = 0; i < len; i++) {
        r = &robots[i];
        pos.x = (r->pos.x + (r->dir.x * steps)) % (int) w;
        if (pos.x < 0) {
            pos.x += w;
        }
        pos.y = (r->pos.y + (r->dir.y * steps)) % (int) h;
        if (pos.y < 0) {
            pos.y += h;
        }

        if (pos.x < wmid) {
            if (pos.y < hmid) {
                quadrants[0]++;
            } else if (pos.y > hmid) {
                quadrants[2]++;
            }
        } else if (pos.x > wmid) {
            if (pos.y < hmid) {
                quadrants[1]++;
            } else if (pos.y > hmid) {
                quadrants[3]++;
            }
        }
    }

    return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

static uint32_t move_robots(Robot *robots, size_t len, size_t w, size_t h)
{
    Robot *r;
    Vec2 pos;

    for (size_t i = 0; i < len; i++) {
        r = &robots[i];
        pos.x = r->pos.x + r->dir.x;
        pos.y = r->pos.y + r->dir.y;

        r->pos.x = pos.x % (int) w;
        if (r->pos.x < 0) {
            r->pos.x += w;
        }
        r->pos.y = pos.y % (int) h;
        if (r->pos.y < 0) {
            r->pos.y += h;
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    Robot robots[MAX_ROBOTS];
    size_t len = 0;

    FILE *fp = fopen("input.txt", "r");
    assert(fp != NULL);
    while (!feof(fp)) {
        Robot *r = &robots[len++];
        if (fscanf(fp, "p=%d,%d v=%d,%d\n", &r->pos.x, &r->pos.y, &r->dir.x, &r->dir.y) != 4) {
            len--;
            break;
        }
    }
    fclose(fp);
    assert(len < MAX_ROBOTS);

    char *rows = malloc(101 * 103);
    memset(rows, ' ', 101 * 103);

    char **grid = NULL;
    grid = malloc(103 * sizeof(char *));
    for (size_t i = 0; i < 103; i++) {
        grid[i] = &rows[i * 101];
    }

    size_t moves = 0;
    while (true) {
        for (size_t i = 0; i < len; i++) {
            uint32_t x = robots[i].pos.x;
            uint32_t y = robots[i].pos.y;
            grid[y][x] = '#';
        }

        /* Find a vertical line of 4 robots */
        bool found_shape = false;
        for (size_t i = 0; i < 103 - 4; i++) {
            for (size_t j = 0; j < 101 - 4; j++) {
                if (grid[i][j] == '#' && grid[i][j + 1] == '#' && grid[i][j + 2] == '#' && grid[i][j + 3] == '#' &&
                    grid[i][j] == '#' && grid[i + 1][j] == '#' && grid[i + 2][j] == '#' && grid[i + 3][j] == '#' &&
                    grid[i][j] == '#' && grid[i + 1][j + 1] == '#' && grid[i + 2][j + 2] == '#' && grid[i + 3][j + 3] == '#') {
                    found_shape = true;
                }
            }
        }
        /* If we found a line, wait for keypress */
        if (found_shape) {
            for (size_t i = 0; i < 103; i++) {
                printf("%.101s\n", grid[i]);
            }
            break;
        }

        move_robots(robots, len, 101, 103);
        moves++;
        memset(rows, ' ', 101 * 103);
    }

    free(rows);
    free(grid);

    printf("Part 1: %u\n", simulate_movement(robots, len, 100, 101, 103));
    printf("Part 2: %zu\n", moves);

    return 0;
}
