from itertools import permutations
import itertools
import sys

if __name__ == "__main__":
    with open("input.txt") as fh:
        vertices = []
        edges = {}
        for l in fh.readlines():
            path, cost = l.split(" = ")
            source, dest = path.split(" to ")
            edges[(source, dest)] = int(cost)
            edges[(dest, source)] = int(cost)
            vertices.append(source)
            vertices.append(dest)
        vertices = set(vertices)

    options = permutations(vertices)
    shortest = sys.maxsize
    longest = 0
    for opt in options:
        current = 0
        src = opt[0]
        for dest in opt[1:]:
            current += edges[(src, dest)]
            src = dest
        shortest = min(shortest, current)
        longest = max(longest, current)
    print("Part 1: %d" % shortest)
    print("Part 2: %d" % longest)
