#include <stdio.h>
#include <stdlib.h>
#include <libinput.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
	int children;
	int cats;
	int samoyeds;
	int pomeranians;
	int akitas;
	int vizslas;
	int goldfish;
	int trees;
	int cars;
	int perfumes;
} Sue;

static void blank_sue(Sue *sue)
{
	sue->children = -1;
	sue->cats = -1;
	sue->samoyeds = -1;
	sue->pomeranians = -1;
	sue->akitas = -1;
	sue->vizslas = -1;
	sue->goldfish = -1;
	sue->trees = -1;
	sue->cars = -1;
	sue->perfumes = -1;
}

static void set_value(Sue *sue, char *param, int val)
{
	if (strncmp(param, "chi", 3) == 0)
		sue->children = val;
	else if(strncmp(param, "cat", 3) == 0)
		sue->children = val;
	else if(strncmp(param, "sam", 3) == 0)
		sue->samoyeds = val;
	else if(strncmp(param, "pom", 3) == 0)
		sue->pomeranians = val;
	else if(strncmp(param, "aki", 3) == 0)
		sue->akitas = val;
	else if(strncmp(param, "viz", 3) == 0)
		sue->vizslas = val;
	else if(strncmp(param, "gol", 3) == 0)
		sue->goldfish = val;
	else if(strncmp(param, "tre", 3) == 0)
		sue->trees = val;
	else if(strncmp(param, "car", 3) == 0)
		sue->cars = val;
	else if(strncmp(param, "per", 3) == 0)
		sue->perfumes = val;
	else
		fprintf(stderr, "Bad sue param: %s\n", param);
}

static Sue* goal_sue()
{
	Sue *sue = malloc(sizeof(Sue));
	sue->children = 3;
	sue->cats = 7;
	sue->samoyeds = 2;
	sue->pomeranians = 3;
	sue->akitas = 0;
	sue->vizslas = 0;
	sue->goldfish = 5;
	sue->trees = 3;
	sue->cars = 2;
	sue->perfumes = 1;
	return sue;
}

static Sue* parse_input(Lines *lines)
{
	Sue* all = malloc(sizeof(Sue) * 500);

	char param1[50];
	int val1;
	char param2[50];
	int val2;
	char param3[50];
	int val3;

	for (int i = 0; i < 500; ++i) {
		blank_sue(&all[i]);
		sscanf(
		       lines->lines[i],
		       "Sue %*s %s %d, %s %d, %s %d",
		       param1,
		       &val1,
		       param2,
		       &val2,
		       param3,
		       &val3
		      );
		set_value(&all[i], param1, val1);
		set_value(&all[i], param2, val2);
		set_value(&all[i], param3, val3);
		
	}
	return all;
}

static bool match_sues_part1(Sue *goal, Sue *other)
{
	bool match = true;
	if (other->children > 0)
		match = match && goal->children == other->children;
	if (other->cats > 0)
		match = match && goal->cats == other->cats;
	if (other->samoyeds > 0)
		match = match && goal->samoyeds == other->samoyeds;
	if (other->pomeranians > 0)
		match = match && goal->pomeranians == other->pomeranians;
	if (other->akitas > 0)
		match = match && goal->akitas == other->akitas;
	if (other->vizslas > 0)
		match = match && goal->vizslas == other->vizslas;
	if (other->goldfish > 0)
		match = match && goal->goldfish == other->goldfish;
	if (other->trees > 0)
		match = match && goal->trees == other->trees;
	if (other->cars > 0)
		match = match && goal->cars == other->cars;
	if (other->perfumes > 0)
		match = match && goal->perfumes == other->perfumes;
	return match;
}

static bool match_sues_part2(Sue *goal, Sue *other)
{
	bool match = true;
	if (other->children > -1)
		match = match && goal->children == other->children;
	if (other->cats > -1)
		match = match && goal->cats < other->cats;
	if (other->samoyeds > -1)
		match = match && goal->samoyeds == other->samoyeds;
	if (other->pomeranians > -1)
		match = match && goal->pomeranians > other->pomeranians;
	if (other->akitas > -1)
		match = match && goal->akitas == other->akitas;
	if (other->vizslas > -1)
		match = match && goal->vizslas == other->vizslas;
	if (other->goldfish > -1)
		match = match && goal->goldfish > other->goldfish;
	if (other->trees > -1)
		match = match && goal->trees < other->trees;
	if (other->cars > -1)
		match = match && goal->cars == other->cars;
	if (other->perfumes > -1)
		match = match && goal->perfumes == other->perfumes;
	return match;
}

int main()
{
	FILE *fp = fopen("input.txt", "r");
	Lines *lines = lines_read(fp);
	fclose(fp);
	Sue *sues = parse_input(lines);
	lines_free(lines);
	Sue *goal = goal_sue();
	for (int i = 0; i < 500; ++i) {
		if (match_sues_part1(goal, &sues[i])) {
			printf("Part 1: %d\n", i+1);
		}
		if (match_sues_part2(goal, &sues[i])) {
			printf("Part 2: %d\n", i+1);
		}
	}
	free(goal);
	free(sues);
}
