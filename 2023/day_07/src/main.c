#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <libinput.h>

typedef uint32_t u32;
typedef uint64_t u64;

typedef enum {
    FIVE_OF_KIND,
    FOUR_OF_KIND,
    FULL_HOUSE,
    THREE_OF_KIND,
    TWO_PAIR,
    ONE_PAIR,
    HIGH_CARD
} HandType;

typedef struct {
    char hand[6];
    u64 bid;
    HandType type;
} Hand;

static u64 JVAL = 11;

static u64 val(char c)
{
    switch (c) {
        case 'A':
            return 14;
        case 'K':
            return 13;
        case 'Q':
            return 12;
        case 'J':
            return JVAL;
        case 'T':
            return 10;
        case '9':
            return 9;
        case '8':
            return 8;
        case '7':
            return 7;
        case '6':
            return 6;
        case '5':
            return 5;
        case '4':
            return 4;
        case '3':
            return 3;
        case '2':
            return 2;
        default:
            fprintf(stderr, "Bad value: %c\n", c);
            exit(1);
    }
}

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

static HandType hand_type(Hand *hand)
{
    u64 fbuf[15] = {0};

    for (char *c = hand->hand; *c != '\0'; c++) {
        fbuf[val(*c)]++;
    }

    size_t card_count = 0;
    size_t high_count = 0;
    for (size_t i = 2; i < 15; ++i)  {
        if (fbuf[i] > 0) {
            card_count++;
            high_count = MAX(high_count, fbuf[i]);
        }
    }

    assert(card_count > 0);
    if (card_count == 1) {
        return FIVE_OF_KIND;
    } else if (card_count == 2) {
        if (high_count == 4) {
            return FOUR_OF_KIND;
        } else {
            return FULL_HOUSE;
        }
    } else if (card_count == 3) {
        if (high_count == 3) {
            return THREE_OF_KIND;
        } else {
            return TWO_PAIR;
        }
    } else if (card_count == 4) {
        return ONE_PAIR;
    } else {
        return HIGH_CARD;
    }
}

static HandType joker_hand_type(Hand *hand)
{
    u64 fbuf[15] = {0};

    for (char *c = hand->hand; *c != '\0'; c++) {
        fbuf[val(*c)]++;
    }

    size_t card_count = 0;
    size_t low_count = 0u - 1;
    size_t high_count = 0;
    size_t joker_count = fbuf[val('J')];

    if (joker_count != 5) {
        fbuf[val('J')] = 0;
    }

    for (size_t i = 0; i < 15; ++i)  {
        if (fbuf[i] > 0) {
            card_count++;
            high_count = MAX(high_count, fbuf[i]);
            low_count = MIN(low_count, fbuf[i]);
        }
    }

    if (joker_count < 5) {
        if (high_count >= 3 && high_count == joker_count) {
            high_count += low_count;
        } else if (joker_count > 0) {
            high_count += joker_count;
        }
    }

    assert(card_count > 0);
    if (card_count == 1) {
        assert(high_count == 5 || high_count + joker_count == 5);
        return FIVE_OF_KIND;
    } else if (card_count == 2) {
        if (high_count == 4) {
            return FOUR_OF_KIND;
        } else {
            return FULL_HOUSE;
        }
    } else if (card_count == 3) {
        if (high_count == 3) {
            return THREE_OF_KIND;
        } else {
            return TWO_PAIR;
        }
    } else if (card_count == 4) {
        return ONE_PAIR;
    } else {
        return HIGH_CARD;
    }
}

static int cmp_hand(const void *h1, const void *h2)
{
    const Hand *a = h1;
    const Hand *b = h2;

    if (strcmp(a->hand, b->hand) == 0) {
        return 0;
    }

    if (a->type == b->type) {
        for (size_t i = 0; i < 6; ++i) {
            u64 c1 = val(a->hand[i]);
            u64 c2 = val(b->hand[i]);

            if (c1 == c2) {
                continue;
            } else {
                return c1 - c2;
            }
        }
        return 0;
    } else if (a->type < b->type) {
        return 1;
    } else {
        return -1;
    }
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    int result = 0;

    Lines *lines = lines_read_file("input.txt");
    if (!lines) {
        perror("lines_read_file");
        result = -1;
        goto out;
    }

    u32 hand_len = lines->size;
    Hand *hands = malloc(hand_len * sizeof(Hand));

    for (size_t i = 0; i < hand_len; ++i) {
        Hand *hand = &hands[i];
        char *line = lines->lines[i];

        strncpy(hand->hand, line, 5);
        hand->hand[5] = '\0';
        hand->bid = strtol(&line[5], NULL, 10);
        hand->type = hand_type(hand);

    }

    qsort(hands, lines->size, sizeof(Hand), &cmp_hand);

    u64 winnings = 0;
    for (size_t i = 0; i < hand_len; ++i) {
        Hand *hand = &hands[i];
        winnings += (i + 1) * hand->bid;
    }

    printf("Part 1: %lu\n", winnings);

    /* Enter joker mode */
    JVAL = 1;

    /* Recalculate all hands with joker logic */
    for (size_t i = 0; i < hand_len; ++i) {
        Hand *hand = &hands[i];
        hand->type = joker_hand_type(hand);
    }

    qsort(hands, lines->size, sizeof(Hand), &cmp_hand);
    winnings = 0;
    for (size_t i = 0; i < hand_len; ++i) {
        Hand *hand = &hands[i];
        winnings += (i + 1) * hand->bid;
    }
    printf("Part 2: %lu\n", winnings);

    free(hands);
out:
    if (lines) {
        lines_free(lines);
    }
    return result;
}
