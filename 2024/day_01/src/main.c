#include <libinput.h>
#include <libcollections.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

static int cmpnum(const void *a, const void *b)
{
    return *(int *)a - *(int *)b;
}

static void part1(const Array *a, const Array *b)
{
    Array list1, list2;
    array_copy(&list1, a);
    array_copy(&list2, b);

    qsort(list1.array, list1.used, sizeof(int64_t), cmpnum);
    qsort(list2.array, list2.used, sizeof(int64_t), cmpnum);

    uint64_t total_distance = 0;
    for (size_t i = 0; i < list1.used; i++) {
        total_distance += labs(list1.array[i] - list2.array[i]);
    }

    array_free(&list1);
    array_free(&list2);

    printf("Part 1: %ld\n", total_distance);
}

static void part2(Array *a, Array *b)
{
    uint64_t sim_score = 0;
    for (size_t i = 0; i < a->used; i++) {
        size_t occurences = 0;
        for (size_t j = 0; j < b->used; j++) {
            if (a->array[i] == b->array[j]) {
                occurences++;
            }
        }
        sim_score += a->array[i] * occurences;
    }

    printf("Part 2: %ld\n", sim_score);
}

int main(int argc, char **argv)
{
    int ret_val = 0;

    FILE *fp;
    Array list1;
    Array list2;

    array_init(&list1, 1024);
    array_init(&list2, 1024);

    int64_t a, b;

    fp = fopen(argv[1], "r");
    if (!fp) {
        fp = stdin;
    }

    /* Read the input */
    while (fscanf(fp, "%ld   %ld\n", &a, &b) == 2) {
        array_add(&list1, a);
        array_add(&list2, b);
    }

    part1(&list1, &list2);
    part2(&list1, &list2);

    array_free(&list1);
    array_free(&list2);

    if (fp != stdin) {
        fclose(fp);
    }
    return ret_val;
}
