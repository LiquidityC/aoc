import sys

def part1(crabs):
    min_cost = sys.maxsize
    for i in range(max(crabs)):
        cost = sum([abs(crab - i) for crab in crabs])
        min_cost = min(min_cost, cost)
    print("Part 1: %d" % min_cost)

def part2(crabs):
    min_cost = sys.maxsize
    for i in range(max(crabs)):
        cost = sum([(abs(crab-i)*(abs(crab-i)+1)/2) for crab in crabs])
        min_cost = min(cost, min_cost)
    print("Part 2: %d" % min_cost)

if __name__ == "__main__":
    with open("input.txt") as fh:
        crabs = [int(n) for n in fh.read().split(",")]

    part1(crabs)
    part2(crabs)
