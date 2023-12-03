#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <libinput.h>
#include <libcollections.h>

typedef uint32_t u32;

typedef struct {
    u32 r;
    u32 g;
    u32 b;
} Set;

typedef struct {
    u32 id;
    Set sets[10];
    size_t scount;
} Game;

static void parse_game(Game *game, const char *line)
{
    char buffer[256];

    if (sscanf(line, "Game %d: %[^\n]", &game->id, buffer) != 2) {
        perror("sscanf");
        return;
    }

    size_t index = 0;
    char *saveptr1, *saveptr2;
    char *setstr = strtok_r(buffer, ";", &saveptr1);
    while (setstr) {
        Set *set = &game->sets[index++];
        memset(set, 0, sizeof(Set));

        char *colstr = strtok_r(setstr, ",", &saveptr2);
        while (colstr) {
            u32 count;
            char color[10];
            if (sscanf(colstr, "%u %s", &count, color) != 2) {
                perror("sscanf");
                return;
            }

            if (strcmp(color, "red") == 0) {
                set->r = count;
            } else if (strcmp(color, "green") == 0) {
                set->g = count;
            } else if (strcmp(color, "blue") == 0) {
                set->b = count;
            }
            colstr = strtok_r(NULL, ",", &saveptr2);
        }
        setstr = strtok_r(NULL, ";", &saveptr1);
    }
    game->scount = index;
}

static void parse_games(Game *games, Lines *lines)
{
    for (size_t i = 0; i < lines->size; i++) {
        parse_game(games + i, lines->lines[i]);
    }
}

#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

static void part1(Game *games, size_t count)
{
    u32 sum = 0;

    for (size_t i = 0; i < count; ++i) {
        Game *game = games + i;
        bool possible = true;
        for (size_t j = 0; j < game->scount; ++j) {
            Set *set = game->sets + j;
            if (set->r > MAX_RED) {
                possible = false;
                break;
            } else if (set->g > MAX_GREEN) {
                possible = false;
                break;
            } else if (set->b > MAX_BLUE) {
                possible = false;
                break;
            }
        }

        if (possible) {
            sum += game->id;
        }
    }

    printf("Part 1: %u\n", sum);
}

#define MAX(a, b) ((a > b) ? a : b)

static void part2(Game *games, size_t count)
{
    uint64_t sum = 0;

    for (size_t i = 0; i < count; ++i) {
        u32 red = 0, green = 0, blue = 0;
        Game *game = games + i;
        for (size_t j = 0; j < game->scount; ++j) {
            Set *set = game->sets + j;
            red = MAX(set->r, red);
            green = MAX(set->g, green);
            blue = MAX(set->b, blue);
        }

        sum += red * green * blue;
    }

    printf("Part 2: %lu\n", sum);
}

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    Game *games = NULL;
    int result = 0;

    Lines *lines = lines_read_file("input.txt");
    if (!lines) {
        perror("lines_read_file");
        result = -1;
        goto out;
    }

    games = calloc(lines->size, sizeof(Game));
    if (games == NULL) {
        perror("calloc");
        goto out;
    }

    parse_games(games, lines);

    part1(games, lines->size);
    part2(games, lines->size);

out:
    if (games) {
        free(games);
    }
    if (lines) {
        lines_free(lines);
    }
    return result;
}
