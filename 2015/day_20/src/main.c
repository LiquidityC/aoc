#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

static size_t find_divisors(uint64_t *divisors, uint64_t number)
{
    size_t index = 0;
    uint64_t root = (uint64_t) floor(sqrt((double) number));
    for (uint64_t i = 1; i <= root; ++i) {
        if ((number % i) == 0) {
            divisors[index++] = i;
            if (number/i != i) {
                divisors[index++] = number/i;
            }
        }
    }
    return index;
}

static uint64_t calc(uint64_t number)
{
    uint64_t div_size = 2 * (uint64_t) floor(sqrt((double) number));
    uint64_t *divisors = malloc(div_size * sizeof(uint64_t));
    size_t div_cnt = find_divisors(divisors, number);
    uint64_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        sum += divisors[i] * 10;
    }
    free(divisors);
    return sum;
}

static uint64_t alt_calc(uint64_t number)
{
    uint64_t div_size = 2 * (uint64_t) floor(sqrt((double) number));
    uint64_t *divisors = malloc(div_size * sizeof(uint64_t));
    size_t div_cnt = find_divisors(divisors, number);
    uint64_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        if (number < divisors[i] * 50)
            sum += divisors[i] * 11;
    }
    free(divisors);
    return sum;
}

int main()
{
    assert(calc(1) == 10);
    assert(calc(2) == 30);
    assert(calc(3) == 40);
    assert(calc(4) == 70);
    assert(calc(5) == 60);
    assert(calc(6) == 120);
    assert(calc(7) == 80);
    assert(calc(8) == 150);
    assert(calc(9) == 130);
    assert(calc(7) == 80);
    assert(calc(9) == 130);

    uint64_t h = 1;
    while (true) {
        printf("\rChecking: %lu", h);
        if (calc(h) >= 34000000) {
            printf("\r\x1b[2KPart 1: %lu\n", h);
            break;
        }
        h++;
    }
    h = 1;
    while (true) {
        printf("\rChecking: %lu", h);
        if (alt_calc(h) >= 34000000) {
            printf("\r\x1b[2KPart 2: %lu\n", h);
            break;
        }
        h++;
    }
    return 0;
}
