#include <libcollections.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int x;
	int y;
} Vector;

static void move_tail(Vector *head, Vector *tail)
{
	if (abs(head->x - tail->x) <= 1 && abs(head->y - tail->y) <= 1)
		return;

	/* Check row alignment */
	if (head->y != tail->y) {
		if (head->y > tail->y)
			tail->y += 1;
		else
			tail->y -= 1;
	}

	/* Check col alignment */
	if (head->x != tail->x) {
		if (head->x > tail->x)
			tail->x += 1;
		else
			tail->x -= 1;
	}
}

static inline void dir_to_step(char dir, Vector *step)
{
	step->x = 0;
	step->y = 0;
	switch (dir) {
		case 'U':
			step->y = 1;
			break;
		case 'D':
			step->y = -1;
			break;
		case 'R':
			step->x = 1;
			break;
		case 'L':
			step->x = -1;
			break;
	};
}

int main(void)
{
	Vector knots[10];
	memset(&knots, 0, 10 * sizeof(Vector));

	char key_buf[10];
	HashTable *visited1 = ht_create();
	HashTable *visited2 = ht_create();

	char dir;
	int steps;
	Vector step;
	FILE *fp = fopen("input.txt", "r");
	while (fscanf(fp, "%c %d\n", &dir, &steps) != EOF) {
		dir_to_step(dir, &step);

		for (int i = 0; i < steps; ++i) {

			/* Move the head */
			knots[0].x += step.x;
			knots[0].y += step.y;

			/* Move the tails */
			for (int t = 1; t < 10; ++t) {
				move_tail(&knots[t-1], &knots[t]);
			}

			/* Track the second and last knot */
			snprintf(key_buf, 10, "%dx%d", knots[1].x, knots[1].y);
			ht_set(visited1, key_buf, &knots);
			snprintf(key_buf, 10, "%dx%d", knots[9].x, knots[9].y);
			ht_set(visited2, key_buf, &knots);
		}
	}

	printf("Part 1: %lu\n", ht_length(visited1));
	printf("Part 2: %lu\n", ht_length(visited2));

	ht_destroy(visited1);
	ht_destroy(visited2);

	return 0;
}
