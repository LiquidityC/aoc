#include <stdio.h>
#include <libinput.h>
#include <libcollections.h>
#include <stdbool.h>
#include <string.h>

static void step(Matrix *matrix, bool locked_corners)
{
    Matrix *new_m = matrix_new(matrix->width, matrix->height);
    for (int y = 0; y < matrix->height; ++y) {
        for (int x = 0; x < matrix->width; ++x) {
            int neighbors[8];
            int len = matrix_neighbors8(matrix, &neighbors[0], x, y);
            int sum = 0;
            for (int i = 0; i < len; ++i) {
                sum += neighbors[i];
            }
            if (locked_corners && len == 3)
                new_m->data[y][x] = 1;
            else if (matrix->data[y][x] == 0 && sum == 3)
                new_m->data[y][x] = 1;
            else if (matrix->data[y][x] == 1 && (sum == 2 || sum == 3))
                new_m->data[y][x] = 1;
            else
                new_m->data[y][x] = 0;
        }
    }
    matrix_copy(new_m, matrix);
    matrix_free(new_m);
}

int main(void)
{
    FILE* fp = fopen("input.txt", "r");
    Lines* lines = lines_read(fp);
    Matrix* matrix = matrix_new(lines->length, lines->size);
    for (int i = 0; i < lines->size; ++i) {
        for (int j = 0; j < strlen(lines->lines[i]); ++j) {
            char c = lines->lines[i][j];
            if (c == '.')
                matrix->data[i][j] = 0;
            else
                matrix->data[i][j] = 1;
        }
    }
    lines_free(lines);
    Matrix* orig = matrix_new(matrix->width, matrix->height);
    matrix_copy(matrix, orig);

    for (int i = 0; i < 100; ++i) {
        step(matrix, false);
    }
    printf("Part 1: %d\n", matrix_sum(matrix));
    matrix_copy(orig, matrix);
    for (int i = 0; i < 100; ++i) {
        step(matrix, true);
    }
    printf("Part 2: %d\n", matrix_sum(matrix));

    matrix_free(matrix);
    matrix_free(orig);
}
