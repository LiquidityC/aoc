#include <assert.h>
#include <libinput.h>
#include <libcollections.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint32_t sum;
    bool do_mul;
} State;

typedef struct {
    const char *match;
    const size_t mlen;
    void (*exec)(State *state, char **pos);
} Operation;

#define OP_CREATE(pattern, func) { .match = pattern, .mlen = strlen(pattern), .exec = func }

static void mul_exec(State *state, char **pos)
{
    uint32_t a, b;

    *pos += 4;

    if (!state->do_mul) {
        return;
    }

    a = strtol(*pos, pos, 10);
    if (a == 0 || **pos != ',') {
        return;
    }
    *pos += 1;
    b = strtol(*pos, pos, 10);
    if (b == 0 || **pos != ')') {
        return;
    }

    state->sum += a * b;
}

static void do_exec(State *state, char **pos)
{
    state->do_mul = true;
    *pos += 4;
}

static void dont_exec(State *state, char **pos)
{
    state->do_mul = false;
    *pos += 6;
}

static void part1(Lines *lines)
{
    const Operation mul_operation = OP_CREATE("mul(", mul_exec);
    State state = { .sum = 0, .do_mul = true };

    for (size_t i = 0; i < lines->size; i++) {
        char *line = lines->lines[i];
        char *pos = line;

        while (*pos != '\0') {
            if (strncmp(mul_operation.match, pos, mul_operation.mlen) == 0) {
                mul_operation.exec(&state, &pos);
            } else {
                pos += 1;
            }
        }
    }

    printf("Part 1: %u\n", state.sum);
}

static void part2(Lines *lines)
{
    const Operation ops[] = {
        OP_CREATE("mul(", mul_exec),
        OP_CREATE("do()", do_exec),
        OP_CREATE("don't()", dont_exec),
    };

    State state = { .sum = 0, .do_mul = true };

    for (size_t i = 0; i < lines->size; ++i) {
        char *pos = lines->lines[i];

        while (*pos != '\0') {
            bool match = false;
            for (size_t j = 0; j < sizeof(ops) / sizeof(ops[0]); ++j) {
                if (strncmp(ops[j].match, pos, ops[j].mlen) == 0) {
                    ops[j].exec(&state, &pos);
                    match = true;
                }
            }
            if (!match) {
                pos += 1;
            }
        }
    }

    printf("Part 2: %u\n", state.sum);
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
