#include <stdio.h>
#include <string.h>

typedef struct {
    char name[50];
    int capacity;
    int durability;
    int flavor;
    int texture;
    int calories;
} Ingredient;

typedef struct {
    int score;
    int calories;
} IngredientScore;

#define max(X, Y) (((X) > (Y)) ? (X) : (Y))

IngredientScore score(
          const Ingredient *a,
          int a_amt,
          const Ingredient *b,
          int b_amt,
          const Ingredient *c,
          int c_amt,
          const Ingredient *d,
          int d_amt
         )
{
    int capacity = a->capacity*a_amt + b->capacity*b_amt + c->capacity*c_amt + d->capacity*d_amt;
    int durability = a->durability*a_amt + b->durability*b_amt + c->durability*c_amt + d->durability*d_amt;
    int flavor = a->flavor*a_amt + b->flavor*b_amt + c->flavor*c_amt + d->flavor*d_amt;
    int texture = a->texture*a_amt + b->texture*b_amt + c->texture*c_amt + d->texture*d_amt;
    int calories = a->calories*a_amt + b->calories*b_amt + c->calories*c_amt + d->calories*d_amt;

    capacity = max(capacity, 0);
    durability = max(durability, 0);
    flavor = max(flavor, 0);
    texture = max(texture, 0);
    calories = max(calories, 0);

    IngredientScore result;
    result.score = capacity * durability * flavor * texture;
    result.calories = calories;
    return result;
}

void solve(Ingredient* ings)
{
    int best = 0;
    int calorie_best = 0;
    IngredientScore current;

    for (int i = 1; i < 100; i++) {
        for (int j = 1; j < 100-i; j++) {
            for (int k = 1; k < 100-i-j; k++) {
                int l = 100-k-j-i;
                current = score(&ings[0], i, &ings[1], j, &ings[2], k, &ings[3], l);
                if (best < current.score) {
                    best = current.score;
                }
                if (current.calories == 500 && calorie_best < current.score) {
                    calorie_best = current.score;
                }
            }
        }
    }

    printf("Part 1: %d\n", best);
    printf("Part 2: %d\n", calorie_best);
}

int main(int argc, char** argv)
{
    Ingredient ingredients[4];
    FILE *fh = fopen("input.txt", "r");
    if (fh == NULL) {
        fprintf(stderr, "Failed to read input.txt");
        return -1;
    }

    for (int i = 0; i < 4; i++) {
        fscanf(fh, "%s capacity %d, durability %d, flavor %d, texture %d, calories %d",
               ingredients[i].name,
               &ingredients[i].capacity,
               &ingredients[i].durability,
               &ingredients[i].flavor,
               &ingredients[i].texture,
               &ingredients[i].calories
               );
        int name_len = strlen(ingredients[i].name);
        ingredients[i].name[name_len - 1] = '\0';
    }
    fclose(fh);

    solve(ingredients);
}
