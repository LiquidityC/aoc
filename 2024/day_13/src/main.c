#include <libinput.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_MACHINES 400


typedef struct {
    int ax, ay, bx, by, px, py;
} Machine;

static uint64_t calculate_cost(Machine *machines, size_t len, uint64_t target_offset)
{
    static const double EPSILON = 0.000001;

    uint64_t total_cost = 0;
    for (size_t i = 0; i < len; i++) {
        Machine *machine = &machines[i];
        double px = machine->px + target_offset;
        double py = machine->py + target_offset;
        double ax = machine->ax;
        double ay = machine->ay;
        double bx = machine->bx;
        double by = machine->by;

        double a = (by * px - bx * py) / (by * ax - bx * ay);
        double b = (px - ax * a) / bx;

        if (fabs(a - round(a)) < EPSILON && fabs(b - round(b)) < EPSILON) {
            total_cost += a * 3 + b;
        }
    }
    return total_cost;
}

int main(int argc, char **argv)
{
    Machine machines[MAX_MACHINES];
    size_t len = 0;

    Lines *lines;
    size_t row = 0;

    lines = lines_read_file("input.txt");

    while (row < lines->size) {
        Machine *machine = &machines[len++];
        assert(len < MAX_MACHINES);

        assert(sscanf(lines->lines[row++], "Button A: X+%d, Y+%d", &machine->ax, &machine->ay) == 2);
        assert(sscanf(lines->lines[row++], "Button B: X+%d, Y+%d", &machine->bx, &machine->by) == 2);
        assert(sscanf(lines->lines[row++], "Prize: X=%d, Y=%d", &machine->px, &machine->py) == 2);
        
        /* Skip the empty line */
        row++;
    }
    lines_free(lines);

    printf("Part 1: %lu\n", calculate_cost(machines, len, 0));
    printf("Part 2: %lu\n", calculate_cost(machines, len, 10000000000000));

    return 0;
}
