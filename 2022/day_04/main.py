count1 = 0
count2 = 0
with open("input.txt") as fh:
    for l in fh.readlines():
        pairs = l.split(",")
        ranges = map(lambda x: list(map(int, x.split("-"))), pairs)
        s1, s2 = map(lambda r: set(range(r[0], r[1]+1)), ranges)

        count1 += 1 if s2.issubset(s1) or s1.issubset(s2) else 0
        count2 += 1 if len(s1.intersection(s2)) > 0 else 0
print("Part 1: %d" % count1)
print("Part 2: %d" % count2)
