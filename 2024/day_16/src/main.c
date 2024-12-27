#include <libcollections.h>
#include <libinput.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum cell_type {
    CELL_EMPTY,
    CELL_WALL
} CellType;

typedef struct vec2 {
    size_t x;
    size_t y;
} Vec2;

typedef Vec2 Pos;

typedef struct cell {
    CellType type;
    Vec2 dir;
} Cell;

typedef struct map {
    Cell **cells;
    Cell *rows;
    size_t w;
    size_t h;
} Map;

typedef struct reindeer {
    Vec2 pos;
    Vec2 dir;
} Reindeer;

typedef struct node {
    uint32_t score;
    Vec2 dir;
    size_t x;
    size_t y;
} Node;

/* Path storage */
typedef struct path {
    uint32_t previous[4];
    size_t previous_count;
} Path;

/* A min-heap */
typedef struct heap {
    Node *array;
    size_t size;
    size_t capacity;
} Heap;

static void swap(Node *a, Node *b)
{
    Node tmp = *a;
    *a = *b;
    *b = tmp;
}

static void hp_init(Heap *heap, size_t capacity)
{
    heap->array = calloc(capacity, sizeof(Node));
    heap->size = 0;
    heap->capacity = capacity;
}

static void hp_heapify(Heap *heap, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && heap->array[left].score < heap->array[largest].score) {
        largest = left;
    }

    if (right < heap->size && heap->array[right].score < heap->array[largest].score) {
        largest = right;
    }

    if (largest != i) {
        swap(&heap->array[i], &heap->array[largest]);
        hp_heapify(heap, largest);
    }
}

static void hp_insert(Heap *heap, uint32_t score, Vec2 dir, size_t x, size_t y)
{
    assert(heap->size < heap->capacity);

    heap->size++;
    int i = heap->size - 1;

    Node *node = &heap->array[i];
    node->score = score;
    node->dir = dir;
    node->x = x;
    node->y = y;

    while (i != 0 && heap->array[(i - 1)/2].score > heap->array[i].score) {
        swap(&heap->array[i], &heap->array[(i - 1)/2]);
        i = (i - 1) / 2;
    }
}

static void hp_pop(Heap *heap, Node *node)
{
    assert(heap->size > 0);

    *node = heap->array[0];
    if (heap->size == 1) {
        heap->size--;
    } else {
        heap->array[0] = heap->array[heap->size - 1];
        heap->size--;
        hp_heapify(heap, 0);
    }
}

static void parse_map(Map *map, Lines *lines, Pos *start, Pos *end)
{
    for (size_t i = 0; i < lines->size; i++) {
        for (size_t j = 0; j < lines->length; j++) {
            char c = lines->lines[i][j];
            Cell *cell = &map->cells[i][j];
            switch (c) {
                case '#':
                    cell->type = CELL_WALL;
                    break;
                case '.':
                    cell->type = CELL_EMPTY;
                    break;
                case 'S':
                    cell->type = CELL_EMPTY;
                    cell->dir = (Vec2) {1, 0};
                    start->x = j;
                    start->y = i;
                    break;
                case 'E':
                    cell->type = CELL_EMPTY;
                    end->x = j;
                    end->y = i;
                    break;
                default:
                    assert(false);
            }
        }
    }
}

static uint32_t find_path(Map *map, Pos start, Pos end)
{
    const Vec2 dirs[] = {
        {0, -1},
        {0, 1},
        {-1, 0},
        {1, 0}
    };

    Heap heap;
    uint32_t *fscore;
    size_t node_count;

    node_count = map->w * map->h;

    Path *paths = calloc(node_count, sizeof(Path));

    hp_init(&heap, node_count);
    hp_insert(&heap, 0, (Vec2) { 1, 0 }, start.x, start.y);

    fscore = alloca(node_count * sizeof(uint32_t));

    memset(fscore, 0xFF, node_count * sizeof(uint32_t));

    uint32_t result = 0;
    while (heap.size > 0) {
        Node node;
        hp_pop(&heap, &node);

        if (node.x == end.x && node.y == end.y) {
            result = node.score;
            goto out;
        }

        for (size_t i = 0; i < 4; i++) {
            Vec2 dir = dirs[i];
            size_t x = node.x + dir.x;
            size_t y = node.y + dir.y;

            if (x >= map->w || y >= map->h) {
                continue;
            }

            Cell *next = &map->cells[y][x];
            if (next->type == CELL_WALL) {
                continue;
            }

            uint32_t score = node.score + 1;
            Vec2 *current_dir = &node.dir;
            if (current_dir->x != dir.x || current_dir->y != dir.y) {
                if ((current_dir->x == dir.x) || (current_dir->y == dir.y)) {
                    /* Half rotation */
                    score += 2000;
                } else {
                    /* Quarter rotation */
                    score += 1000;
                }
            }

            Path *pn = &paths[y * map->w + x];
            if (score < fscore[y * map->w + x]) {
                pn->previous[0] = node.y * map->w + node.x;
                pn->previous_count = 1;
                fscore[y * map->w + x] = score;
                hp_insert(&heap, score, dir, x, y);
            } else if (score == fscore[y * map->w + x]) {
                pn->previous[pn->previous_count++] = node.y * map->w + node.x;
                hp_insert(&heap, score, dir, x, y);
            }
        }
    }

out:
    free(heap.array);
    free(paths);
    return result;
}

int main(int argc, char **argv)
{
    Lines *lines;
    Map map;
    Pos start, end;

    lines = lines_read_file("input.txt");

    /* Allocate the map */
    map.w = lines->length;
    map.h = lines->size;
    map.cells = alloca(lines->size * sizeof(Cell *));
    map.rows = alloca(lines->size * lines->length * sizeof(Cell));
    for (size_t i = 0; i < map.h; i++) {
        map.cells[i] = &map.rows[i * map.w];
    }

    parse_map(&map, lines, &start, &end);

    for (size_t i = 0; i < map.h; i++) {
        for (size_t j = 0; j < map.w; j++) {
            Cell cell = map.cells[i][j];
            switch (cell.type) {
                case CELL_WALL:
                    printf("#");
                    break;
                case CELL_EMPTY:
                    printf(".");
                    break;
                default:
                    assert(false);
            }
        }
        printf("\n");
    }
    lines_free(lines);

    printf("Part 1: %u\n", find_path(&map, start, end));

    return 0;
}
