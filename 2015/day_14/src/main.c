#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <libinput.h>

const char *input_file = "input.txt";

typedef struct {
    char name[10];
    int speed;
    int time;
    int rest;
    int distance;
    int resting;
    int moving;
    int score;
} Deer;

Deer* read_deer(Lines *lines)
{
    Deer *deer = (Deer*) malloc(sizeof(Deer)*lines->size);

    int index = 0;
    while (index < lines->size) {
        Deer *tmp = &deer[index];
        tmp->distance = 0;
        tmp->resting = 0;
        tmp->moving = 0;
        tmp->score = 0;
        sscanf(lines->lines[index],
               "%s can fly %d km/s for %d seconds, but then must rest for %d %*s",
              tmp->name,
               &tmp->speed,
               &tmp->time,
               &tmp->rest
              );
        index++;
    }
    return deer;
}

void part1(Deer *deer, int deer_count, int time)
{
    int top_distance = 0;
    Deer *leader = NULL;

    for (int i = 0; i < deer_count; ++i) {
        int distance = 0;
        Deer *tmp = &deer[i];
        int full_cycles = time/(tmp->time + tmp->rest);
        distance += full_cycles * tmp->speed * tmp->time;
        int remainder = time % (tmp->time + tmp->rest);
        if (remainder < tmp->time) {
            distance += remainder * tmp->speed;
        } else {
            distance += tmp->time * tmp->speed;
        }
        if (distance > top_distance) {
            top_distance = distance;
            leader = &deer[i];
        }
    }

    printf("Part 1: %d\n", top_distance);
}

void part2(Deer *deer, int deer_count, int time)
{
    int best_score = 0;
    for (int step = 0; step < time; step++) {
        int leading_distance = 0;
        for (int i = 0; i < deer_count; i++) {
            Deer *current = &deer[i];
            if (current->resting == 0) {
                current->distance += current->speed;
                current->moving += 1;
                if (current->moving >= current->time) {
                    current->resting = current->rest;
                    current->moving = 0;
                }
            } else {
                current->resting -= 1;
            }

            if (current->distance > leading_distance) {
                leading_distance = current->distance;
            }
        }
        for (int i = 0; i < deer_count; i++) {
            if (leading_distance == deer[i].distance) {
                deer[i].score += 1; 
            }
            if (deer[i].score > best_score) {
                best_score = deer[i].score;
            }
        }
    }
    printf("Part 2: %d\n", best_score);
}

int main(int argc, char** argv)
{
    FILE *fp = fopen(input_file, "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s", input_file);
    }

    Lines* lines = read_lines(fp);
    int deer_count = lines->size;
    fclose(fp);

    Deer *deer = read_deer(lines);
    lines_free(lines);

    part1(deer, deer_count, 2503);
    part2(deer, deer_count, 2503);

    free(deer);
}
