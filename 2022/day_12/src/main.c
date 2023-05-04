#include <libcollections.h>
#include <libinput.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct Pos {
	uint32_t x;
	uint32_t y;
} Pos;

static void remove_item_from(Pos *open_nodes, size_t index, size_t *len)
{
	*len -= 1;
	for (size_t i = index; i < *len; ++i) {
		open_nodes[i] = open_nodes[i+1];
	}
}

static uint32_t get_item(uint32_t *vec, Pos *pos, size_t width)
{
	return vec[pos->x + (pos->y*width)];
}

static void set_item(uint32_t *vec, Pos *pos, size_t width, uint32_t value)
{
	vec[pos->x + (pos->y*width)] = value;
}

static size_t find_best_node(Pos *open_nodes, size_t open_nodes_len, uint32_t *fscore, size_t width)
{
	size_t best_node;
	uint32_t best_score = UINT32_MAX;

	for (size_t i = 0; i < open_nodes_len; ++i) {
		uint32_t score = get_item(fscore, &open_nodes[i], width);
		if (score < best_score) {
			best_node = i;
			best_score = score;
		}
	}
	return best_node;
}

static bool has_node(Pos *open_nodes, Pos *node, size_t open_nodes_len)
{
	for (size_t i = 0; i < open_nodes_len; ++i) {
		if (open_nodes[i].x == node->x && open_nodes[i].y == node->y) {
			return true;
		}
	}
	return false;
}

static size_t get_valid_neighbors(Matrix *m, Pos *p, Pos *buffer)
{
	size_t i = 0;
	int32_t max_height = m->data[p->y][p->x] + 1;
	if (p->y > 0)
		if (m->data[p->y-1][p->x] <= max_height)
			buffer[i++] = (Pos) { p->x, p->y - 1 };
	if (p->y + 1 < m->height)
		if (m->data[p->y+1][p->x] <= max_height)
			buffer[i++] = (Pos) { p->x, p->y + 1 };
	if (p->x > 0)
		if (m->data[p->y][p->x-1] <= max_height)
			buffer[i++] = (Pos) { p->x - 1, p->y };
	if (p->x + 1 < m->width) {
		if (m->data[p->y][p->x+1] <= max_height)
			buffer[i++] = (Pos) { p->x + 1, p->y };
	}
	return i;

}

static uint32_t score(Pos *n, Pos *g)
{
	return abs((int)n->x - (int)g->x) +  abs((int)n->y - (int)g->y);
}

static size_t count_steps(Pos *current, Pos *came_from, Pos *start, size_t width)
{
	size_t steps = 0;
	while (current->x != start->x || current->y != start->y) {
		current = &came_from[current->x + (current->y*width)];
		steps++;
	}
	return steps;
}

static size_t find_path(Matrix *m, Pos *start, Pos *end)
{
	size_t steps = 0;
	Pos *open_nodes = calloc(m->width * m->height, sizeof(uint32_t));
	size_t open_nodes_len = 0;

	size_t width = m->width;

	uint32_t *fscore = malloc(width * m->height * sizeof(uint32_t));
	memset(fscore, UINT32_MAX - 10, width * m->height * sizeof(uint32_t));
	uint32_t *gscore = malloc(width * m->height * sizeof(uint32_t));
	memset(gscore, UINT32_MAX - 10, width * m->height * sizeof(uint32_t));
	Pos *came_from = calloc(width * m->height, sizeof(Pos));

	open_nodes[open_nodes_len++] = *start;

	set_item(fscore, start, width, score(start, end));
	set_item(gscore, start, width, 0);

	Pos neighbors[4];

	while (open_nodes_len > 0) {
		size_t best = find_best_node(open_nodes, open_nodes_len, fscore, width);
		Pos node = open_nodes[best];
		if (node.x == end->x && node.y == end->y) {
			steps = count_steps(&node, came_from, start, width);
			goto out;
		}
		remove_item_from(open_nodes, best, &open_nodes_len);
		size_t ncount = get_valid_neighbors(m, &node, neighbors);
		for (size_t i = 0; i < ncount; ++i) {
			Pos n = neighbors[i];
			uint32_t tscore = get_item(gscore, &node, width) + 1;
			if (tscore < get_item(gscore, &n, width)) {
				if (abs((int) node.x - (int) n.x) > 1) {
					assert(abs((int) node.x - (int) n.x) < 2);
				}
				assert(abs((int) node.y - (int) n.y) < 2);
				came_from[n.x + (n.y*width)] = node;
				set_item(gscore, &n, width, tscore);
				set_item(fscore, &n, width, tscore + score(&n, end));
				if (!has_node(open_nodes, &n, open_nodes_len))
					open_nodes[open_nodes_len++] = n;
			}
		}
	}

out:
	free(open_nodes);
	free(came_from);
	free(fscore);
	free(gscore);
	return steps;
}

int main(void)
{
	FILE *fp = fopen("input.txt", "r");
	Lines *lines = lines_read(fp);
	fclose(fp);

	Matrix *matrix = matrix_new(lines->length, lines->size);
	Pos start, end;
	for (size_t i = 0; i < lines->size; ++i) {
		for (size_t j = 0; j < lines->length; ++j) {
			if (lines->lines[i][j] == 'S') {
				start.x = j;
				start.y = i;
				matrix->data[i][j] = 0;
			} else if (lines->lines[i][j] == 'E') {
				end.x = j;
				end.y = i;
				matrix->data[i][j] = (int32_t) 'z' - (int32_t) 'a';
			} else {
				matrix->data[i][j] = (int32_t) lines->lines[i][j] - (int32_t) 'a';
			}
		}
	}

	printf("Part 1: %lu\n", find_path(matrix, &start, &end));

	size_t scenic_path = UINT32_MAX;
	for (size_t i = 0; i < matrix->height; ++i) {
		for (size_t j = 0; j < matrix->width; ++j) {
			if (matrix->data[i][j] == 0) {
				Pos p = { j, i };
				size_t result = find_path(matrix, &p, &end);
				if (result > 0 && result < scenic_path)
					scenic_path = result;
			}
		}
	}
	printf("Part 2: %lu\n", scenic_path);

	matrix_free(matrix);
	lines_free(lines);
}
