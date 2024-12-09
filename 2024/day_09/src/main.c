#include <assert.h>
#include <libinput.h>
#include <libcollections.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static void parse_to_array(Array *blocks, const char *line, size_t len)
{
    size_t file_count = 0;

    bool is_file = true;
    for (size_t i = 0; i < len; i++) {
        uint8_t count = line[i] - '0';

        if (is_file) {
            for (size_t j = 0; j < count; j++) {
                array_add(blocks, file_count);
            }
            file_count++;
        } else {
            for (size_t j = 0; j < count; j++) {
                array_add(blocks, -1);
            }
        }
        is_file = !is_file;
    }
}

static uint64_t i64_checksum(int64_t *mem, size_t mem_size)
{
    uint64_t sum = 0;

    for (size_t i = 0; i < mem_size; i++) {
        if (mem[i] >= 0) {
            sum += mem[i] * i;
        }
    }

    return sum;
}

static void part1(const char *line, size_t len)
{
    Array blocks;

    int64_t *mem;
    size_t mem_size;

    int64_t *first;
    int64_t *last;

    array_init(&blocks, len);

    parse_to_array(&blocks, line, len);

    /* Copy an array to work on */
    mem = malloc(blocks.used * sizeof(int64_t));
    memcpy(mem, blocks.array, blocks.used * sizeof(int64_t));
    mem_size = blocks.used;

    /* Free the dynamic array */
    array_free(&blocks);

    first = mem;
    last = mem + mem_size - 1;

    while (first < last) {
        /* Find last non-empty memory block */
        while (last > first && *last < 0) {
            last--;
        }

        /* Find first empty memory block */
        while (first < last && *first >= 0) {
            first++;
        }

        if (first == last) {
            break;
        }

        /* Swap first and last */
        *first = *last;
        *last = -1;
    }

    printf("Part 1: %lu\n", i64_checksum(mem, mem_size));
    free(mem);
}

struct mem_block {
    int64_t *block;
    size_t size;
    size_t used;
};

static struct mem_block* parse_to_mem_blocks(const char *line, size_t len)
{
    size_t fcount = 0;
    bool is_file = true;
    struct mem_block *mem;

    mem = calloc(len, sizeof(struct mem_block));
    assert(mem != NULL);

    for (size_t i = 0; i < len; i++) {
        struct mem_block *block = &mem[i];
        uint8_t count = line[i] - '0';

        block->size = count;
        block->block = malloc(count * sizeof(int64_t));
        if (is_file) {
            for (size_t j = 0; j < count; j++) {
                block->block[j] = fcount;
            }
            block->used = count;
            fcount++;
        } else {
            block->size = count;
            block->used = 0;
        }
        is_file = !is_file;
    }

    return mem;
}

static uint64_t mem_block_checksum(struct mem_block *mem, size_t len)
{
    uint64_t sum = 0;

    size_t index = 0;
    for (size_t i = 0; i < len; i++) {
        struct mem_block *block = &mem[i];
        for (size_t j = 0; j < block->used; j++) {
            sum += block->block[j] * index++;
        }
        index += block->size - block->used;
    }

    return sum;
}

static inline void mem_block_free(struct mem_block *mem, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        free(mem[i].block);
    }
    free(mem);
}

static void part2(const char *lines, size_t len)
{
    struct mem_block *mem;
    struct mem_block *first, *last;

    mem = parse_to_mem_blocks(lines, len);

    first = mem;
    last = mem + len - 1;

    while (first < last) {
        /* Find last non-empty memory block */
        while (first < last && last->used == 0) {
            last--;
        }

        /* Find first block with remaining space that can fit the last block */
        while (first < last && (first->size - first->used) < last->used) {
            first++;
        }

        /* No move to make. Skip last, reset first and continue */
        if (first >= last) {
            first = mem;
            last--;
            continue;
        }

        /* Copy the block */
        for (size_t i = 0; i < last->used; i++) {
            first->block[first->used++] = last->block[i];
            last->block[i] = -1;
        }
        last->used = 0;

        /* Reset first pointer */
        first = mem;
    }

    printf("Part 2: %lu\n", mem_block_checksum(mem, len));
    mem_block_free(mem, len);
}

int main(int argc, char **argv)
{
    Lines *lines;

    /* Read the input data */
    lines = lines_read_file("input.txt");

    assert(lines->size == 1);

    part1(lines->lines[0], lines->length);
    part2(lines->lines[0], lines->length);

    lines_free(lines);

    return 0;
}
