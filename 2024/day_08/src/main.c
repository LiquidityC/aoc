#include <assert.h>
#include <libinput.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char symbol;
    bool antenna;
    bool antinode;
} Cell;

typedef struct {
    Cell **cells;
    size_t w;
    size_t h;
} Grid;

static void parse_lines(Grid *grid, Lines *lines)
{
    for (size_t i = 0; i < lines->size; i++) {
        for (size_t j = 0; j < lines->length; j++) {
            char c = lines->lines[i][j];
            Cell *cell = &grid->cells[i][j];
            cell->symbol = c;
            if (c == '.')
                continue;

            cell->antenna = true;
        }
    }
}

static inline bool in_grid(Grid *grid, int x, int y)
{
    return x >= 0 && x < grid->w && y >= 0 && y < grid->h;
}

static void set_antinode(Grid *grid, int x, int y)
{
    if (in_grid(grid, x, y)) {
        grid->cells[y][x].antinode = true;
    }
}

static int gcd(int a, int b)
{
    while (b != 0) {
        int t = b;
        b = a % b;
        // 6 % 12 = 6
        a = t;
    }

    return a;
}

static void repeat_antinode(Grid *grid, int x, int y, int dx, int dy)
{
    int nx = x + dx;
    int ny = y + dy;

    while (in_grid(grid, nx, ny)) {
        set_antinode(grid, nx, ny);
        nx += dx;
        ny += dy;
    }
}

static void place_antinodes_for(Grid *grid, int y, int x, bool repeating)
{

    Cell *current = &grid->cells[y][x];

    for (size_t i = 0; i < grid->h; ++i) {
        for (size_t j = 0; j < grid->w; ++j) {
            if (j == x && i == y)
                continue;

            Cell *cell = &grid->cells[i][j];
            if (cell->antenna && cell->symbol == current->symbol) {
                int dx = x - j;
                int dy = y - i;

                /* Place nodes (not repeating for part 1, repeating for part 2) */
                if (!repeating) {
                    set_antinode(grid, x + dx, y + dy);
                    set_antinode(grid, j - dx, i - dy);
                } else {
                    int div = gcd(dx, dy);
                    dx = dx / div;
                    dy = dy / div;
                    repeat_antinode(grid, x, y, dx, dy);
                    repeat_antinode(grid, x, y, -dx, -dy);
                }
            }
        }
    }
}

static void part1(Grid *grid)
{
    /* For each antenna find manhattan distance (mdist) to every matching
     * antenna. The antinode will be placed +mdist from the found antenna and
     * -mdist from the current antenna. */

    for (size_t i = 0; i < grid->h; ++i) {
        for (size_t j = 0; j < grid->w; ++j) {
            Cell *cell = &grid->cells[i][j];
            if (!cell->antenna)
                continue;

            place_antinodes_for(grid, i, j, false);
        }
    }

    uint32_t count = 0;
    for (size_t i = 0; i < grid->h; ++i) {
        for (size_t j = 0; j < grid->w; ++j) {
            Cell *cell = &grid->cells[i][j];
            count += cell->antinode ? 1 : 0;
        }
    }

    printf("Part 1: %u\n", count);
}

static void part2(Grid *grid)
{
    /* For each antenna find a matching antenna. Find the manhattan distance
     * (mdist). Normalize the manhattan distance (gcd). Place antinodes from
     * original antenna according to the normalized mdist until off grid. */

    for (size_t i = 0; i < grid->h; ++i) {
        for (size_t j = 0; j < grid->w; ++j) {
            Cell *cell = &grid->cells[i][j];
            if (!cell->antenna)
                continue;

            place_antinodes_for(grid, i, j, true);
        }
    }

    uint32_t count = 0;
    for (size_t i = 0; i < grid->h; ++i) {
        for (size_t j = 0; j < grid->w; ++j) {
            Cell *cell = &grid->cells[i][j];
            count += cell->antinode ? 1 : 0;
        }
    }

    printf("Part 2: %u\n", count);
}

int main(int argc, char **argv)
{
    Lines *lines;
    Grid grid;

    /* Read the input data */
    lines = lines_read_file("input.txt");

    /* Allocate the grid */
    grid.cells = malloc(lines->size * sizeof(Cell*));
    for (size_t i = 0; i < lines->size; i++) {
        grid.cells[i] = calloc(lines->length, sizeof(Cell));
    }
    grid.h = lines->size;
    grid.w = lines->length;

    /* Convert lines to grid */
    parse_lines(&grid, lines);
    lines_free(lines);

    part1(&grid);

    /* Note: The grid still has antinodes from part 1 but this has no effect on
     * the outcome of part 2 */
    part2(&grid);

    /* Deallocate everything */
    for (size_t i = 0; i < grid.h; i++) {
        free(grid.cells[i]);
    }
    free(grid.cells);

    return 0;
}
