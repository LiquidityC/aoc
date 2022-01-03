#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdbool.h>

typedef struct {
    char *label;
    uint32_t cost;
    uint32_t damage;
    uint32_t armor;
} Item;

typedef struct {
    uint32_t value;
    uint32_t id;
    int32_t hp;
    uint32_t damage;
    uint32_t armor;
} Character;

static Item Weapons[7] = {
    { "Dagger",        8,     4,       0 },
    { "Shortsword",   10,     5,       0 },
    { "Warhammer",    25,     6,       0 },
    { "Longsword",    40,     7,       0 },
    { "Greataxe",     74,     8,       0 },
    { NULL,            0,      0,       0 },
};

static Item Armor[7] = {
    { "Leather",      13,     0,       1 },
    { "Chainmail",    31,     0,       2 },
    { "Splintmail",   53,     0,       3 },
    { "Bandedmail",   75,     0,       4 },
    { "Platemail",   102,     0,       5 },
    { "None",          0,     0,       0 },
    { NULL,           0,      0,       0 },
};

static Item Rings[8] = {
    { "Damage+1",    25,      1,       0 },
    { "Damage+2",    50,      2,       0 },
    { "Damage+3",   100,      3,       0 },
    { "Defense+1",   20,      0,       1 },
    { "Defense+2",   40,      0,       2 },
    { "Defense+3",   80,      0,       3 },
    { "None",         0,      0,       0 },
    { NULL,           0,      0,       0 },
};

static Character boss = { 0, 2, 104, 8, 1 };

uint32_t fight(Character *a,  Character *b)
{
    int32_t dmg = a->damage - b->armor;
    dmg = dmg > 0 ? dmg : 1;
    b->hp -= dmg;
    if (b->hp <= 0) {
        return a->id;
    }
    else
        return fight(b, a);
}

int main() {
    uint32_t min_cost = INT_MAX;
    uint32_t max_cost = 0;
    Item *min_combo[4];
    Item *max_combo[4];
    Character player;
    player.id = 1;
    for (Item *w = Weapons; w->label != NULL; ++w) {
        for (Item *a = Armor; a->label != NULL; ++a) {
            for (Item *r1 = Rings; r1->label != NULL; ++r1) {
                for (Item *r2 = Rings; r2->label != NULL; ++r2) {
                    if (r1 == r2 && r1->cost != 0) {
                        continue;
                    }
                    player.hp = 100;
                    player.value = w->cost + a->cost + r1->cost + r2->cost;
                    player.damage = w->damage + a->damage + r1->damage + r2->damage;
                    player.armor = w->armor + a->armor + r1->armor + r2->armor;

                    if (r1 != r2) {
                        int32_t orig_boss_hp = boss.hp;
                        if (fight(&player, &boss) == 1) {
                            if (player.value < min_cost) {
                                min_combo[0] = w;
                                min_combo[1] = a;
                                min_combo[2] = r1;
                                min_combo[3] = r2;
                                min_cost = player.value;
                            }
                        } else {
                            if (player.value > max_cost) {
                                max_combo[0] = w;
                                max_combo[1] = a;
                                max_combo[2] = r1;
                                max_combo[3] = r2;
                                max_cost = player.value;
                            }
                        }
                        boss.hp = orig_boss_hp;
                    }
                }
            }
        }
    }
    printf("Cheapest win:\n");
    for (size_t i = 0; i < 4; ++i) {
        Item *item = min_combo[i];
        printf("\x1b[32m%-15s \x1b[33m%3u \x1b[34m%3u \x1b[31m%3u\x1b[0m\n", item->label, item->cost, item->damage, item->armor);
    }
    printf("\n");
    printf("Priciest loss:\n");
    for (size_t i = 0; i < 4; ++i) {
        Item *item = max_combo[i];
        printf("\x1b[32m%-15s \x1b[33m%3u \x1b[34m%3u \x1b[31m%3u\x1b[0m\n", item->label, item->cost, item->damage, item->armor);
    }
    printf("\n");
    printf("Part 1: %d\n", min_cost);
    printf("Part 2: %d\n", max_cost);
}
