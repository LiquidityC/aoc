#include <stdlib.h>
#include "libcollections.h"
#include "util.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

static int max(int a, int b)
{
    return a > b ? a : b;
}

static int min(int a, int b)
{
    return a < b ? a : b;
}

Matrix* matrix_new(unsigned int width,
                   unsigned int height)
{
    Matrix *m = ec_malloc(sizeof(Matrix));
    m->width = width;
    m->height = height;
    m->data = ec_malloc(sizeof(int*) * height);
    for (int i = 0; i < height; ++i) {
        m->data[i] = ec_calloc(width, sizeof(int));
    }
    return m;
}

void matrix_copy(Matrix* src, Matrix* dest) {
    assert(dest->height == src->height && dest->width == src->width);
    for (int i = 0; i < dest->height; ++i) {
        for (int j = 0; j < dest->width; ++j) {
            dest->data[i][j] = src->data[i][j];
        }
    }
}

int matrix_sum(Matrix* m)
{
    int sum = 0;
    for (int i = 0; i < m->height; ++i) {
        for (int j = 0; j < m->width; ++j) {
            sum += m->data[i][j];
        }
    }
    return sum;
}

int matrix_neighbors8(Matrix* matrix, int *buf, uint32_t x, uint32_t y)
{
    int xmin = max(0, x-1);
    int xmax = min(matrix->width-1, x+1);
    int ymin = max(0, y-1);
    int ymax = min(matrix->height-1, y+1);

    int index = 0;
    for (int i = ymin; i <= ymax; ++i) {
        for (int j = xmin; j <= xmax; ++j) {
            if (i == y && j == x)
                continue;
            buf[index++] = matrix->data[i][j];
        }
    }
    return index;
}

void matrix_print(Matrix* m)
{
    for (int i = 0; i < m->height; ++i) {
        for (int j = 0; j < m->width; ++j) {
            printf("%d", m->data[i][j]);
        }
        printf("\n");
    }
}

void matrix_free(Matrix *matrix)
{
    for (int i = 0; i < matrix->height; ++i) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    free(matrix);
}
