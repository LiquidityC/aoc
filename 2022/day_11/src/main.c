#include <libinput.h>
#include <libcollections.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MONKEY_COUNT 10

typedef struct item_list {
	uint64_t item;
	struct item_list *next;
} item_list;

typedef struct {
	item_list *items;
	char operator;
	uint64_t modifier;
	uint64_t divider;
	size_t true_target;
	size_t false_target;
	uint64_t inspect_count;
} Monkey;

static uint64_t operation(uint64_t a, char operator, uint64_t b)
{
	if (b == 0) {
		b = a;
	}
	switch (operator) {
		case '+':
			return a + b;
		case '*':
			return a * b;
		default:
			fprintf(stderr, "Invalid operator: %c\n", operator);
			exit(-1);
	}
}

static item_list* parse_items(char *items)
{
	item_list *head = calloc(1, sizeof(item_list));
	item_list *it;

	char *token = strtok(items, ", ");
	head->item = atoi(token);
	while ((token = strtok(NULL, ", ")) != NULL) {
		uint64_t item = atoi(token);
		it = head;
		while (it->next != NULL)
			it = it->next;
		it->next = calloc(1, sizeof(item_list));
		it->next->item = item;
	}
	return head;
}

static size_t parse_input(Monkey *monkeys)
{
	FILE *fh = fopen("input.txt", "r");
	Lines *lines = lines_read(fh);
	fclose(fh);
	size_t mcount = 0;

	for (int i = 0; i < lines->size; i += 7) {
		Monkey *m = &monkeys[mcount];

		/* Read items */
		m->items = parse_items(lines->lines[i+1]+18);

		/* Parse operation */
		m->operator = *(lines->lines[i+2]+23);
		char *modifier = lines->lines[i+2]+25;
		if (strcmp(modifier, "old") == 0) {
			m->modifier = 0;
		} else {
			m->modifier = atoi(modifier);
		}

		/* Parse divider */
		m->divider = atoi(lines->lines[i+3]+21);

		/* Parse targets */
		m->true_target = atoi(lines->lines[i+4]+29);
		m->false_target = atoi(lines->lines[i+5]+30);

		mcount++;
	}
	lines_free(lines);
	return mcount;
}

static void throw_item_to_monkey(Monkey *m, uint64_t item)
{
	if (m->items == NULL) {
		m->items = calloc(1, sizeof(item_list));
		m->items->item = item;
	} else {
		item_list *it = m->items;
		while (it->next != NULL) {
			it = it->next;
		}
		it->next = calloc(1, sizeof(item_list));
		it->next->item = item;
	}

}

static void turn(Monkey *monkeys, size_t index, uint64_t lcm)
{
	Monkey *m = &monkeys[index];
	while (m->items != NULL) {
		uint64_t item = m->items->item;

		/* Do the monkey business */
		item = operation(item, m->operator, m->modifier);
		if (lcm == 0)
			item /= 3;
		else
			item %= lcm;
		if ((item % m->divider) == 0) {
			throw_item_to_monkey(&monkeys[m->true_target], item);
		} else {
			throw_item_to_monkey(&monkeys[m->false_target], item);
		}

		/* Step the iterator */
		item_list *it = m->items;
		m->items = m->items->next;
		free(it);
		m->inspect_count++;
	}
}

static void monkey_round(Monkey *monkeys, size_t mcount, uint64_t lcm)
{
	for (size_t i = 0; i < mcount; ++i) {
		turn(monkeys, i, lcm);
	}
}

static uint64_t get_monkey_business_level(Monkey *monkeys, size_t mcount)
{
	uint64_t a = 0;
	uint64_t b = 0;

	for (size_t i = 0; i < mcount; ++i) {
		if (monkeys[i].inspect_count > a) {
			if (a > b)
				b = a;
			a = monkeys[i].inspect_count;
		} else if (monkeys[i].inspect_count > b) {
			b = monkeys[i].inspect_count;
		}
	}
	return a*b;
}

static uint64_t run(bool relax, size_t rounds)
{
	Monkey monkeys[MONKEY_COUNT];
	memset(monkeys, 0, MONKEY_COUNT * sizeof(Monkey));

	size_t mcount = parse_input(monkeys);

	/* LCM prod (dividers are all primes so very simple formula) */
	uint64_t lcm = 0;
	if (!relax) {
		lcm = 1;
		for (size_t i = 0; i < mcount; ++i) {
			lcm *= monkeys[i].divider;
		}
	}

	for (size_t i = 0; i < rounds; ++i) {
		monkey_round(monkeys, mcount, lcm);
	}

	size_t result = get_monkey_business_level(monkeys, mcount);

	/* Clean up memory */
	for (size_t i = 0; i < mcount; ++i) {
		item_list *item = monkeys[i].items;
		while (item != NULL) {
			item_list *next = item->next;
			free(item);
			item = next;
		}
	}
	return result;
}

int main(void)
{
	printf("Part 1: %lu\n", run(true, 20));
	printf("Part 2: %lu\n", run(false, 10000));
	return 0;
}
