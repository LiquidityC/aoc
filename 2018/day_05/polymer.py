def reduce(polymer):
    lowers = sorted(set(polymer.lower()))
    uppers = [c.upper() for c in lowers]

    prev = None
    while prev != polymer:
        prev = polymer
        for p, q in zip(lowers, uppers):
            polymer = polymer.replace(p + q, "")
            polymer = polymer.replace(q + p, "")
    return polymer


with open("input.txt") as fh:
    polymer = fh.readline().strip()

print("Part 1:", len(reduce(polymer)))
