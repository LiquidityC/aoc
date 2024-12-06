#include <assert.h>
#include <libinput.h>
#include <libcollections.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct Guard {
    int x;
    int y;
    struct {
        int x;
        int y;
    } dir;
} Guard;

typedef struct Cell {
    bool visited;
    bool obstacle;
    int8_t dx;
    int8_t dy;
    char symbol;
} Cell;

static_assert(sizeof(Cell) <= sizeof(uint64_t), "Cell size must be <= 64 bits");

static void part1(Matrix *matrix, Guard *guard)
{
    while (true) {
        Cell *cell = (Cell*) &matrix->data[guard->y][guard->x];
        cell->visited = true;
        cell->dx = guard->dir.x;
        cell->dy = guard->dir.y;

        int next_x = guard->x + guard->dir.x;
        int next_y = guard->y + guard->dir.y;

        if (next_x < 0 || next_x >= matrix->height || next_y < 0 || next_y >= matrix->width) {
            break;
        }

        Cell *next_cell = (Cell*) &matrix->data[next_y][next_x];
        if (next_cell->obstacle) {
            /* Rotate clockwise 90 degrees */
            int tmp = guard->dir.x;
            guard->dir.x = -guard->dir.y;
            guard->dir.y = tmp;
            continue;
        }
        guard->x = next_x;
        guard->y = next_y;
    }

    uint32_t count = 0;
    for (size_t i = 0; i < matrix->height; ++i) {
        for (size_t j = 0; j < matrix->width; ++j) {
            Cell *cell = (Cell*) &matrix->data[i][j];
            if (cell->visited) {
                count += 1;
            }
        }
    }

    printf("Part 1: %u\n", count);
}

static bool is_cycle(const Matrix *matrix, Guard *guard)
{
    while (true) {
        Cell *cell = (Cell*) &matrix->data[guard->y][guard->x];

        if (cell->visited) {
            if (cell->dx == guard->dir.x && cell->dy == guard->dir.y) {
                return true;
            }
        } else {
            /* Only update the recorded trail if we haven't visited this cell before */
            cell->visited = true;
            cell->dx = guard->dir.x;
            cell->dy = guard->dir.y;
        }

        int next_x = guard->x + guard->dir.x;
        int next_y = guard->y + guard->dir.y;

        if (next_x < 0 || next_x >= matrix->height || next_y < 0 || next_y >= matrix->width) {
            return false;
        }

        Cell *next_cell = (Cell*) &matrix->data[next_y][next_x];
        if (next_cell->obstacle) {
            /* Rotate clockwise 90 degrees */
            int tmp = guard->dir.x;
            guard->dir.x = -guard->dir.y;
            guard->dir.y = tmp;
            continue;
        }
        guard->x = next_x;
        guard->y = next_y;
    }
}

static void part2(Matrix *matrix, Guard *guard)
{
    /* Setup an original matrix */
    Matrix *base_matrix = matrix_new(matrix->width, matrix->height);
    matrix_copy(matrix, base_matrix);

    for (size_t i = 0; i < base_matrix->height; ++i) {
        for (size_t j = 0; j < base_matrix->width; ++j) {
            Cell *cell = (Cell*) &base_matrix->data[i][j];
            cell->visited = false;
        }
    }

    /* Setup a test matrix to run the cycle algorithm on */
    Matrix *test_matrix = matrix_new(base_matrix->width, base_matrix->height);
    matrix_copy(base_matrix, test_matrix);

    /* Setup a default guard */
    Guard test_guard;
    memcpy(&test_guard, guard, sizeof(Guard));

    uint32_t cycle_count = 0;
    for (size_t i = 0; i < matrix->height; ++i) {
        for (size_t j = 0; j < matrix->width; ++j) {
            Cell *cell = (Cell*) &matrix->data[i][j];
            if (cell->visited) {
                Cell *block_cell = (Cell*) &test_matrix->data[i][j];
                block_cell->obstacle = true;
                if (is_cycle(test_matrix, &test_guard)) {
                    cycle_count++;
                }
                block_cell->obstacle = false;

                /* Reset the matrix and guard */
                matrix_copy(base_matrix, test_matrix);
                memcpy(&test_guard, guard, sizeof(Guard));
            }
        }
    }

    matrix_free(base_matrix);
    matrix_free(test_matrix);

    printf("Part 2: %u\n", cycle_count);
}

int main(int argc, char **argv)
{
    Lines *lines;
    Matrix *matrix;
    Guard guard;

    guard.x = 0;
    guard.y = 0;

    int startx = 0;
    int starty = 0;

    lines = lines_read_file("input.txt");
    matrix = matrix_new(lines->length, lines->size);

    for (size_t i = 0; i < lines->size; ++i) {
        for (size_t j = 0; j < lines->length; ++j) {
            Cell *cell = (Cell*) &matrix->data[i][j];
            switch (lines->lines[i][j]) {
                case '^':
                    startx = j;
                    starty = i;
                    cell->symbol = '.';
                    break;
                case '#':
                    cell->symbol = '#';
                    cell->obstacle = true;
                    break;
                case '.':
                    cell->symbol = '.';
                    break;
                default:
                    fprintf(stderr, "Invalid character");
                    return -1;
                    break;
            }
        }
    }
    lines_free(lines);

    guard.x = startx;
    guard.y = starty;
    guard.dir.x = 0;
    guard.dir.y = -1;
    part1(matrix, &guard);

    guard.x = startx;
    guard.y = starty;
    guard.dir.x = 0;
    guard.dir.y = -1;
    part2(matrix, &guard);

    matrix_free(matrix);

    return 0;
}
