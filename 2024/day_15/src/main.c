#include <libcollections.h>
#include <libinput.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct vec2 {
    int x;
    int y;
} Vec2;

typedef struct cell {
    bool wall;
    bool box;
    bool robot;
    bool lhs;
    bool rhs;
} Cell;

typedef struct grid {
    Cell **cells;
    Cell *cell_data;
    size_t w;
    size_t h;
} Grid;


static void print_grid(Grid *grid);
static void execute_move(Grid *grid, Vec2 pos, const Vec2 *move);
static bool check_move(Grid *grid, Vec2 pos, const Vec2 *move);

static inline Vec2 dir_to_vec(char dir)
{
    switch (dir) {
        case '^':
            return (Vec2){0, -1};
        case 'v':
            return (Vec2){0, 1};
        case '<':
            return (Vec2){-1, 0};
        case '>':
            return (Vec2){1, 0};
        default:
            fprintf(stderr, "Invalid direction: %c\n", dir);
            exit(1);
    };
}

static void parse_grid_from_lines(Grid *grid, Lines *lines, Vec2 *rpos, bool wide)
{
    for (size_t i = 0; i < lines->size; i++) {
        if (lines->lines[i][0] == '\0') {
            break;
        }
        for (size_t j = 0; j < strlen(lines->lines[i]); j++) {
            switch (lines->lines[i][j]) {
                case '#':
                    if (!wide) {
                        grid->cells[i][j].wall = true;
                    } else {
                        grid->cells[i][j * 2].wall = true;
                        grid->cells[i][j * 2 + 1].wall = true;
                    }
                    break;
                case 'O':
                    if (!wide) {
                        grid->cells[i][j].box = true;
                    } else {
                        grid->cells[i][j * 2].box = true;
                        grid->cells[i][j * 2].lhs = true;
                        grid->cells[i][j * 2 + 1].box = true;
                        grid->cells[i][j * 2 + 1].rhs = true;
                    }
                    break;
                case '@':
                    if (!wide) {
                        rpos->x = j;
                        rpos->y = i;
                        grid->cells[i][j].robot = true;
                    } else {
                        rpos->x = j * 2;
                        rpos->y = i;
                        grid->cells[i][j * 2].robot = true;
                    }
                    break;
                case '.':
                    break;
                default:
                    fprintf(stderr, "Invalid character in input file: %c\n", lines->lines[i][j]);
                    exit(1);
            }
        }
    }
}

static void parse_instructions_from_lines(Vec2 *instructions, size_t *len, Lines *lines, size_t start_index)
{
    const size_t max_len = *len;
    size_t ii = 0;

    for (size_t i = start_index; i < lines->size; i++) {
        for (size_t j = 0; j < strlen(lines->lines[i]); j++) {
            instructions[ii++] = dir_to_vec(lines->lines[i][j]);
            assert(ii < max_len);
        }
    }

    *len = ii;
}

static Grid* parse_grid(Lines *lines, Vec2 *rpos, bool wide)
{
    Grid *grid;

    grid = malloc(sizeof(Grid));
    assert(grid != NULL);
    memset(grid, 0, sizeof(Grid));

    /* The grid width should equal the length of the first line */
    grid->w = strlen(lines->lines[0]);

    if (wide) {
        grid->w *= 2;
    }

    /* Find grid height */
    for (size_t i = 0; i < lines->size; i++) {
        if (lines->lines[i][0] == '\0') {
            break;
        }
        grid->h++;
    }

    /* Allocate the grid */
    grid->cells = malloc(grid->h * sizeof(Cell *));
    assert(grid->cells != NULL);
    grid->cell_data = malloc(grid->h * grid->w * sizeof(Cell));
    assert(grid->cell_data != NULL);
    memset(grid->cell_data, 0, grid->h * grid->w * sizeof(Cell));

    /* Parse */
    for (size_t i = 0; i < grid->h; i++) {
        grid->cells[i] = grid->cell_data + i * grid->w;
    }

    /* Parse grid */
    parse_grid_from_lines(grid, lines, rpos, wide);

    return grid;
}

static uint32_t calculate_gps(Grid *grid, bool wide)
{
    uint32_t score = 0;
    for (size_t i = 0; i < grid->h; i++) {
        for (size_t j = 0; j < grid->w; j++) {
            if (grid->cells[i][j].box && !wide) {
                score += 100 * i + j;
            } else if (grid->cells[i][j].lhs) {
                score += 100 * i + j;
            }
        }
    }
    return score;
}

static bool check_move_box(Grid *grid, Vec2 pos, const Vec2 *move)
{
    Cell *cell = &grid->cells[pos.y][pos.x];
    assert(cell->box);

    bool vertical_move = move->y != 0;
    if (vertical_move && cell->rhs) {
        Vec2 np1 = {pos.x - 1, pos.y + move->y};
        Vec2 np2 = {pos.x, pos.y + move->y};
        return check_move(grid, np1, move) && check_move(grid, np2, move);
    } else if (vertical_move && cell->lhs) {
        Vec2 np1 = {pos.x, pos.y + move->y};
        Vec2 np2 = {pos.x + 1, pos.y + move->y};
        return check_move(grid, np1, move) && check_move(grid, np2, move);
    } else {
        /* Normal (part 1) box move */
        Vec2 newpos = {pos.x + move->x, pos.y + move->y};
        return check_move(grid, newpos, move);
    }

    return false;
}

static bool check_move(Grid *grid, Vec2 pos, const Vec2 *move)
{
    Cell *cell = &grid->cells[pos.y][pos.x];
    if (!cell->robot && !cell->box && !cell->wall) {
        return true;
    }

    if (cell->wall) {
        return false;
    }

    if (cell->box) {
        return check_move_box(grid, pos, move);
    } else if (cell->robot) {
        Vec2 newpos = {pos.x + move->x, pos.y + move->y};
        return check_move(grid, newpos, move);
    }

    return false;
}

static inline void execute_move_box(Grid *grid, Vec2 pos, const Vec2 *move)
{
    Cell *cell = &grid->cells[pos.y][pos.x];
    assert(cell->box);

    bool vertical_move = move->y != 0;
    if (vertical_move && cell->rhs) {
        Vec2 np1 = {pos.x - 1, pos.y + move->y};
        Vec2 np2 = {pos.x, pos.y + move->y};
        execute_move(grid, np1, move);
        execute_move(grid, np2, move);

        grid->cells[pos.y][pos.x].box = false;
        grid->cells[pos.y][pos.x].rhs = false;
        grid->cells[pos.y][pos.x - 1].box = false;
        grid->cells[pos.y][pos.x - 1].lhs = false;
        grid->cells[np1.y][np1.x].box = true;
        grid->cells[np2.y][np2.x].box = true;
        grid->cells[np1.y][np1.x].lhs = true;
        grid->cells[np2.y][np2.x].rhs = true;
    } else if (vertical_move && cell->lhs) {
        Vec2 np1 = {pos.x, pos.y + move->y};
        Vec2 np2 = {pos.x + 1, pos.y + move->y};
        execute_move(grid, np1, move);
        execute_move(grid, np2, move);
        grid->cells[pos.y][pos.x].box = false;
        grid->cells[pos.y][pos.x].lhs = false;
        grid->cells[pos.y][pos.x + 1].box = false;
        grid->cells[pos.y][pos.x + 1].rhs = false;
        grid->cells[np1.y][np1.x].box = true;
        grid->cells[np2.y][np2.x].box = true;
        grid->cells[np1.y][np1.x].lhs = true;
        grid->cells[np2.y][np2.x].rhs = true;
    } else {
        /* Normal (part 1) box move */
        Vec2 newpos = {pos.x + move->x, pos.y + move->y};
        execute_move(grid, newpos, move);
        grid->cells[newpos.y][newpos.x].box = true;
        grid->cells[newpos.y][newpos.x].rhs = cell->rhs;
        grid->cells[newpos.y][newpos.x].lhs = cell->lhs;
        cell->box = false;
        cell->rhs = false;
        cell->lhs = false;
    }
}

static void execute_move(Grid *grid, Vec2 pos, const Vec2 *move)
{
    Cell *cell = &grid->cells[pos.y][pos.x];
    if (!cell->robot && !cell->box && !cell->wall) {
        return;
    }

    if (cell->box) {
        return execute_move_box(grid, pos, move);
    } else if (cell->robot) {
        Vec2 newpos = {pos.x + move->x, pos.y + move->y};
        execute_move(grid, newpos, move);
        grid->cells[pos.y][pos.x].robot = false;
        grid->cells[newpos.y][newpos.x].robot = true;
    }
}

static void part1(Grid *grid, Vec2 rpos, const Vec2 *instructions, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        const Vec2 *instr = &instructions[i];
        if (check_move(grid, rpos, instr)) {
            execute_move(grid, rpos, instr);
            rpos.x += instr->x;
            rpos.y += instr->y;
        }
    }

    printf("Part 1: %u\n", calculate_gps(grid, false));
}

static void part2(Grid *grid, Vec2 rpos, const Vec2 *instructions, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        const Vec2 *instr = &instructions[i];
        if (check_move(grid, rpos, instr)) {
            execute_move(grid, rpos, instr);
            rpos.x += instr->x;
            rpos.y += instr->y;
        }
    }

    printf("Part 2: %u\n", calculate_gps(grid, true));
}

[[maybe_unused]]
static void print_grid(Grid *grid)
{
    for (size_t i = 0; i < grid->h; i++) {
        for (size_t j = 0; j < grid->w; j++) {
            if (grid->cells[i][j].lhs) {
                printf("[");
            } else if (grid->cells[i][j].rhs) {
                printf("]");
            } else if (grid->cells[i][j].wall) {
                printf("#");
            } else if (grid->cells[i][j].robot) {
                printf("@");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

static void free_grid(Grid **grid)
{
    free((*grid)->cell_data);
    free((*grid)->cells);
    free(*grid);
    *grid = NULL;
}

int main(int argc, char **argv)
{
    Vec2 *instructions;
    Vec2 rpos = {0, 0};
    Grid *grid;

    Lines *lines = lines_read_file("input.txt");

    size_t index = 0;
    grid = parse_grid(lines, &rpos, false);

    /* Allocate instructions */
    size_t instruction_len = 1 + lines->length * (lines->size - index);
    instructions = calloc(instruction_len, sizeof(Vec2));

    /* Parse instructions */
    size_t start_index = grid->h + 1;
    parse_instructions_from_lines(instructions, &instruction_len, lines, start_index);

    /* Part 1 */
    part1(grid, rpos, instructions, instruction_len);
    free_grid(&grid);

    /* Parse a wide grid */
    index = 0;
    grid = parse_grid(lines, &rpos, true);
    lines_free(lines);

    /* Part 2 */
    part2(grid, rpos, instructions, instruction_len);

    free_grid(&grid);
    free(instructions);

    return 0;
}
