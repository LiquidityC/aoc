def sum_calories(lines):
    elves = []
    elf = 0
    for l in lines:
        if l == "":
            elves.append(elf)
            elf = 0
        else:
            elf += int(l)
    elves.append(elf)
    return elves
if __name__ == "__main__":
    with open("input.txt") as fh:
        data = [l.strip() for l in fh.readlines()]
    calories = sum_calories(data)
    print("Part 1: %d" % max(calories))
    calories.sort()
    print("Part 2: %d" % sum(calories[-3:]))
