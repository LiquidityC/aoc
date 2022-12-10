#include <libinput.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

enum op_type {
	NOOP,
	ADDX,
};

typedef union operation {
	enum op_type type;
	struct {
		enum op_type type;
		uint8_t cycles;
	} noop;
	struct {
		enum op_type type;
		uint8_t cycles;
		int32_t value;
	} addx;
} operation;

static int parse_int(char *num)
{
	if (*num == '-') {
		return -1 * atoi(num+1);
	} else {
		return atoi(num);
	}
}

static operation* get_operations(Lines *lines)
{
	operation *ops = calloc(lines->size, sizeof(operation));
	for (int i = 0; i < lines->size; ++i) {
		if (strcmp("noop", lines->lines[i]) == 0) {
			ops[i].noop.type = NOOP;
			ops[i].noop.cycles = 1;
		} else if (strncmp("addx", lines->lines[i], 4) == 0) {
			int val = parse_int(lines->lines[i]+5);
			ops[i].addx.type = ADDX;
			ops[i].addx.cycles = 2;
			ops[i].addx.value = val;
		} else {
			fprintf(stderr, "Invalid operation: %s\n", lines->lines[i]);
			exit(-1);
		}
	}
	return ops;
}

void draw_to_crt(char *crt, int reg, uint32_t cycle)
{
	/* Check if pixel and cycle overlap */
	int hpos = (int) cycle % 40;
	if (hpos < reg-1 || hpos > reg+1) {
		return;
	}
	crt[cycle] = '#';
}

void run(int opc, operation *ops)
{
	/* Allocate the screen */
	char crt[6*40];
	memset(crt, '.', 40 * 6 * sizeof(char));

	int reg = 1;
	uint32_t cycle = 0;
	uint32_t sample_rate = 20;
	uint32_t signal_strength = 0;
	for (int i = 0; i < opc; ++i) {
		for (size_t pc = 0; pc < ops[i].noop.cycles; ++pc) {
			draw_to_crt(crt, reg, cycle);
			cycle += 1;
			if (cycle == sample_rate) {
				signal_strength += reg * cycle;
				sample_rate += 40;
			}
		}
		if (ops[i].type == ADDX) {
			reg += ops[i].addx.value;
		}
	}
	printf("Part 1: %d\n", signal_strength);
	printf("Part 2:\n");
	for (int i = 0; i < 6*40; ++i) {
		if (i != 0 && i % 40 == 0)
			printf("\n");
		if (crt[i] == '.')
			printf("\x1b[30;40m");
		else
			printf("\x1b[37;47m");
		printf("%c", crt[i]);
		printf("\x1b[0m");
	}
	printf("\n");
}

int main(void)
{
	FILE *fh = fopen("input.txt", "r");
	Lines* lines = lines_read(fh);
	fclose(fh);

	operation *ops = get_operations(lines);
	run(lines->size, ops);

	lines_free(lines);
	free(ops);
	return 0;
}
