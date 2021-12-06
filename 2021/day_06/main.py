from collections import Counter

def order(fish):
    ages = Counter(fish)
    age_count=[0 for _ in range(9)] 
    for age in ages: 
        age_count[age]=ages[age] 
    return age_count

def cycle_day(fishes): 
    next = fishes[1:]
    next.append(fishes[0])
    next[6]+= fishes[0]
    return next

def cycle(fishes, n):
    for _ in range(n):
        fishes = cycle_day(fishes)
    return sum(fishes)

if __name__ == "__main__":
    with open("input.txt") as fh:
        fish = [int(fish) for fish in fh.read().split(',')]

    data = order(fish)
    print("Part 1: %d" % cycle(data, 80))
    print("Part 2: %d" % cycle(data, 256))
