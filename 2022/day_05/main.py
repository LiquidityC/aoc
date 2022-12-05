from copy import deepcopy
from collections import defaultdict

def read_stacks(data, stacks):
    for index, l in enumerate(data):
        if l.strip() == "":
            data = data[index+1:]
            break
        crates = l[1::4]
        for i,c in enumerate(crates):
            if c != " " and not c.isnumeric():
                stacks[i+1].append(c)
    stacks = {i:list(reversed(l)) for i,l in stacks.items()}
    return data, stacks

def run_operations(moves, stacks, multipicking):
    for l in moves:
        splits = list(map(int, l.split()[1::2]))
        c, f, t = splits
        crates = stacks[f][-c:]
        if not multipicking:
            crates.reverse()
        stacks[f] = stacks[f][:-c]
        stacks[t] += crates
    return "".join(map(lambda s: s[1][-1], sorted(stacks.items())))

with open("input.txt") as fh:
    data = [l.rstrip() for l in fh.readlines()]

moves, stacks = read_stacks(data, defaultdict(list))

result = run_operations(moves, deepcopy(stacks), False)
print("Part 1: %s" % result)

result = run_operations(moves, deepcopy(stacks), True)
print("Part 2: %s" % result)
