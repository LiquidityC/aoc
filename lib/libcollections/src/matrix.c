#include <stdlib.h>
#include "libcollections.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

static int64_t max(int64_t a, int64_t b)
{
    return a > b ? a : b;
}

static int64_t min(int64_t a, int64_t b)
{
    return a < b ? a : b;
}

Matrix* matrix_new(size_t width,
                   size_t height)
{
    Matrix *m = ec_malloc(sizeof(Matrix));
    m->width = width;
    m->height = height;
    m->data = ec_malloc(sizeof(int64_t*) * height);
    for (size_t i = 0; i < height; ++i) {
        m->data[i] = ec_calloc(width, sizeof(int64_t));
    }
    return m;
}

void matrix_copy(Matrix* src, Matrix* dest) {
    assert(dest->height == src->height && dest->width == src->width);
    for (size_t i = 0; i < dest->height; ++i) {
        for (size_t j = 0; j < dest->width; ++j) {
            dest->data[i][j] = src->data[i][j];
        }
    }
}

int64_t matrix_sum(Matrix* m)
{
    int64_t sum = 0;
    for (size_t i = 0; i < m->height; ++i) {
        for (size_t j = 0; j < m->width; ++j) {
            sum += m->data[i][j];
        }
    }
    return sum;
}

int64_t matrix_neighbors4(Matrix* matrix, int64_t *buf, size_t x, size_t y)
{
    size_t xmin = max(0, x-1);
    size_t xmax = min(matrix->width-1, x+1);
    size_t ymin = max(0, y-1);
    size_t ymax = min(matrix->height-1, y+1);

    size_t index = 0;
    if (xmax != x)
	    buf[index++] = matrix->data[xmax][y];
    if (xmin != x)
	    buf[index++] = matrix->data[xmin][y];
    if (ymin != y)
	    buf[index++] = matrix->data[x][ymin];
    if (ymax != y)
	    buf[index++] = matrix->data[x][ymax];

    return index;
}

int64_t matrix_neighbors8(Matrix* matrix, int64_t *buf, size_t x, size_t y)
{
    size_t xmin = max(0, x-1);
    size_t xmax = min(matrix->width-1, x+1);
    size_t ymin = max(0, y-1);
    size_t ymax = min(matrix->height-1, y+1);

    int64_t index = 0;
    for (size_t i = ymin; i <= ymax; ++i) {
        for (size_t j = xmin; j <= xmax; ++j) {
            if (i == y && j == x)
                continue;
            buf[index++] = matrix->data[i][j];
        }
    }
    return index;
}

bool matrix_contains_point(Matrix *matrix, int64_t x, int64_t y)
{
	return x >= 0 && x < (int64_t) matrix->width && y >= 0 && y < (int64_t) matrix->height;
}

void matrix_print(Matrix* m)
{
    for (size_t i = 0; i < m->height; ++i) {
        for (size_t j = 0; j < m->width; ++j) {
            printf("%ld", m->data[i][j]);
        }
        printf("\n");
    }
}

void matrix_free(Matrix *matrix)
{
    for (size_t i = 0; i < matrix->height; ++i) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}
