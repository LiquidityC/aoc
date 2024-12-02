#include <libinput.h>
#include <libcollections.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define SIZE 1000
#define COL 8

static void parse(FILE* input, int values[][COL], int* sizes, int size) {
    char buffer[SIZE][50];
    int index = 0;
    while (fgets(buffer[index], sizeof(buffer[index]), input)) {
        char *str = buffer[index];
        size_t i = 0;
        while (*str != '\0') {
            int num = strtol(str, &str, 10);
            if (num == 0) {
                break;
            }
            values[index][i++] = num;
        }
        sizes[index] = i;
        index++;
    }
}

static bool is_safe(int *array, int size, int skip)
{
    int prev = array[0];
    int incr = array[1] - array[0];
    if (skip == 1) {
        incr = array[2] - array[0];
    }
    for (size_t i = 1; i < size; i++) {
        if (skip == i) {
            continue;
        }
        int diff = array[i] - prev;
        if (abs(diff) > 3 || abs(diff) < 1) {
            return false;
        }
        if (incr * diff < 0) {
            return false;
        }
        prev = array[i];
    }
    return true;
}

static void part1(int values[][COL], int* sizes, int size) {
    int total = 0;
    for (size_t i = 0; i < size; i++) {
        if (is_safe(values[i], sizes[i], -1)) {
            total++;
        }
    }
    printf("Part 1: %d\n", total);
}

static void part2(int values[][COL], int* sizes, int size) {
    int total = 0;
    for (size_t i = 0; i < size; ++i) {
        if (is_safe(values[i], sizes[i], -1)) {
            /* Check if the unedited report is safe */
            total++;
            continue;
        } else if (is_safe(values[i] + 1, sizes[i] - 1, -1)) {
            /* Check if removing the first level makes the report safe */
            total++;
            continue;
        }
        for (size_t j = 1; j < size; ++j) {
            /* Check if removing any other level makes the report safe */
            if (is_safe(values[i], sizes[i], j)) {
                total++;
                break;
            }
        }
    }
    printf("Part 2: %d\n", total);
}

int main(int argc, char **argv)
{
    int values[SIZE][COL];
    int sizes[SIZE];
    FILE *fp;

    fp = fopen(argv[1], "r");
    if (!fp) {
        fp = stdin;
    }

    parse(fp, values, sizes, SIZE);
    fclose(fp);

    part1(values, sizes, SIZE);
    part2(values, sizes, SIZE);

    return 0;
}
