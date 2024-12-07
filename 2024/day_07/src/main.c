#include <assert.h>
#include <libinput.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_NUMBERS 20

typedef uint64_t u64;

typedef struct {
    u64 result;
    u64 numbers[MAX_NUMBERS];
    size_t num_count;
} Equation;

static void parse_input(Lines *lines, Equation *equations)
{
    for (size_t i = 0; i < lines->size; ++i) {
        Equation *equation = &equations[i];
        char *line = lines->lines[i];

        equation->result = strtoll(line, &line, 10);
        assert(*line == ':');
        line += 2;

        int num_count = 0;
        while ((equation->numbers[num_count++] = strtoll(line, &line, 10)) != 0) {
            if (*line == '\0') {
                break;
            }
            assert(*line == ' ' || *line == '\0');
            line += 1;
        }
        equation->num_count = num_count;
    }
}

static u64 concat_u64(u64 a, u64 b)
{
    u64 pow = 10;
    while (b >= pow) {
        pow *= 10;
    }
    return a * pow + b;
}

static size_t find_solutions(Equation *e, u64 result, size_t index, bool use_concat)
{
    if (index == e->num_count) {
        return result == e->result ? 1 : 0;
    }

    if (index == 0) {
        result += e->numbers[index++];
    }

    size_t solutions = 0;
    solutions += find_solutions(e, result + e->numbers[index], index + 1, use_concat);
    solutions += find_solutions(e, result * e->numbers[index], index + 1, use_concat);
    if (use_concat) {
        solutions += find_solutions(e, concat_u64(result, e->numbers[index]), index + 1, use_concat);
    }
    return solutions;
}

static void part1(Equation *equations, size_t num_equations)
{
    u64 total = 0;
    for (size_t i = 0; i < num_equations; ++i) {
        if (find_solutions(&equations[i], 0, 0, false) > 0) {
            total += equations[i].result;
        }
    }
    printf("Part 1: %zu\n", total);
}

static void part2(Equation *equations, size_t num_equations)
{
    u64 total = 0;
    for (size_t i = 0; i < num_equations; ++i) {
        if (find_solutions(&equations[i], 0, 0, true) > 0) {
            total += equations[i].result;
        }
    }
    printf("Part 2: %zu\n", total);
}

int main(int argc, char **argv)
{
    Lines *lines;
    Equation *equations;
    size_t num_equations;

    /* Read the input data */
    lines = lines_read_file("input.txt");
    num_equations = lines->size;
    equations = malloc(num_equations * sizeof(Equation));
    parse_input(lines, equations);
    lines_free(lines);

    part1(equations, num_equations);
    part2(equations, num_equations);

    free(equations);

    return 0;
}
