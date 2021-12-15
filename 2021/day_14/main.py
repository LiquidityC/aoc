from collections import defaultdict

def calculate(template, rules, steps):
    pCounts, eCounts = defaultdict(int), defaultdict(int)
    for a,b in zip(template, template[1:]):
        pCounts[a+b] += 1
    for c in template:
        eCounts[c] += 1

    for _ in range(steps):
        for pair, count in pCounts.copy().items():
            for new_pair in [pair[0] + rules[pair], rules[pair] + pair[1]]:
                pCounts[new_pair] += count
            pCounts[pair] -= count
            eCounts[rules[pair]] += count

    return (max(eCounts.values()) - min(eCounts.values()))


if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip() for l in fh.readlines()]
        template = lines[0]
        rules = {}
        for r in lines[2:]:
            rule, insertion = r.split(" -> ")
            rules[rule] = insertion

    print("Part 1: %d" % calculate(template, rules, 10))
    print("Part 2: %d" % calculate(template, rules, 40))
