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

typedef enum cell_type {
    EMPTY,
    WALL,
    BOX,
    LBOX,
    RBOX,
    ROBOT
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
                        grid->cells[i][j] = WALL;
                    } else {
                        grid->cells[i][j * 2] = WALL;
                        grid->cells[i][j * 2 + 1] = WALL;
                    }
                    break;
                case 'O':
                    if (!wide) {
                        grid->cells[i][j] = BOX;
                    } else {
                        grid->cells[i][j * 2] = LBOX;
                        grid->cells[i][j * 2 + 1] = RBOX;
                    }
                    break;
                case '@':
                    if (!wide) {
                        rpos->x = j;
                        rpos->y = i;
                        grid->cells[i][j] = ROBOT;
                    } else {
                        rpos->x = j * 2;
                        rpos->y = i;
                        grid->cells[i][j * 2] = ROBOT;
                    }
                    break;
                case '.':
                    if (!wide) {
                        grid->cells[i][j] = EMPTY;
                    } else {
                        grid->cells[i][j * 2] = EMPTY;
                        grid->cells[i][j * 2 + 1] = EMPTY;
                    }
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
            if (grid->cells[i][j] == BOX || grid->cells[i][j] == LBOX) {
                score += 100 * i + j;
            }
        }
    }
    return score;
}

static bool check_move_box(Grid *grid, Vec2 pos, const Vec2 *move)
{
    Cell cell = grid->cells[pos.y][pos.x];
    assert(cell == BOX || cell == LBOX || cell == RBOX);

    bool vertical_move = move->y != 0;
    if (vertical_move && cell == RBOX) {
        Vec2 np1 = {pos.x - 1, pos.y + move->y};
        Vec2 np2 = {pos.x, pos.y + move->y};
        return check_move(grid, np1, move) && check_move(grid, np2, move);
    } else if (vertical_move && cell == LBOX) {
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
    Cell cell = grid->cells[pos.y][pos.x];
    if (cell == EMPTY) {
        return true;
    } else if (cell == WALL) {
        return false;
    }

    if (cell == BOX || cell == LBOX || cell == RBOX) {
        return check_move_box(grid, pos, move);
    } else if (cell == ROBOT) {
        Vec2 newpos = {pos.x + move->x, pos.y + move->y};
        return check_move(grid, newpos, move);
    }

    return false;
}

static inline void execute_move_box(Grid *grid, Vec2 pos, const Vec2 *move)
{
    Cell cell = grid->cells[pos.y][pos.x];
    assert(cell == BOX || cell == LBOX || cell == RBOX);

    bool vertical_move = move->y != 0;
    if (vertical_move && cell == RBOX) {
        Vec2 np1 = {pos.x - 1, pos.y + move->y};
        Vec2 np2 = {pos.x, pos.y + move->y};
        execute_move(grid, np1, move);
        execute_move(grid, np2, move);

        grid->cells[np1.y][np1.x] = LBOX;
        grid->cells[np2.y][np2.x] = RBOX;
        grid->cells[pos.y][pos.x] = EMPTY;
        grid->cells[pos.y][pos.x - 1] = EMPTY;
    } else if (vertical_move && cell == LBOX) {
        Vec2 np1 = {pos.x, pos.y + move->y};
        Vec2 np2 = {pos.x + 1, pos.y + move->y};
        execute_move(grid, np1, move);
        execute_move(grid, np2, move);
        grid->cells[np1.y][np1.x] = LBOX;
        grid->cells[np2.y][np2.x] = RBOX;
        grid->cells[pos.y][pos.x] = EMPTY;
        grid->cells[pos.y][pos.x + 1] = EMPTY;
    } else {
        Vec2 newpos = {pos.x + move->x, pos.y + move->y};
        execute_move(grid, newpos, move);
        grid->cells[newpos.y][newpos.x] = cell;
        grid->cells[pos.y][pos.x] = EMPTY;
    }
}

static void execute_move(Grid *grid, Vec2 pos, const Vec2 *move)
{
    Cell cell = grid->cells[pos.y][pos.x];
    assert(cell != WALL);
    if (cell == EMPTY) {
        return;
    }

    if (cell == BOX || cell == LBOX || cell == RBOX) {
        return execute_move_box(grid, pos, move);
    } else if (cell == ROBOT) {
        Vec2 newpos = {pos.x + move->x, pos.y + move->y};
        execute_move(grid, newpos, move);
        grid->cells[pos.y][pos.x] = EMPTY;
        grid->cells[newpos.y][newpos.x] = ROBOT;
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
            switch (grid->cells[i][j]) {
                case EMPTY:
                    printf(".");
                    break;
                case WALL:
                    printf("#");
                    break;
                case BOX:
                    printf("O");
                    break;
                case LBOX:
                    printf("[");
                    break;
                case RBOX:
                    printf("]");
                    break;
                case ROBOT:
                    printf("@");
                    break;
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
