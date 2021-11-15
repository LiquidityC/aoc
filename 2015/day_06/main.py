import re
import sys
from collections import defaultdict

def part1(lines):
    matrix = [defaultdict(lambda: False) for _ in range(1001)]
    for line in lines:
        [r1,c1,r2,c2] = [int(x) for x in re.findall(r"\d+", line)]
        for x in range(r1, r2+1):
            for y in range(c1, c2+1):
                if line.startswith("turn on "):
                    matrix[x][y] = True
                elif line.startswith("turn off "):
                    matrix[x][y] = False
                elif line.startswith("toggle "):
                    matrix[x][y] = not matrix[x][y]
                else:
                    print("Bad instruction")
                    sys.exit(1)
    count = 0
    for x in range(0, 1001):
        for y in range(0, 1001):
            if matrix[x][y]:
                count += 1
    print("Part 1: %d" % count)

def part2(lines):
    matrix = [defaultdict(int) for _ in range(1001)]
    for line in lines:
        [r1,c1,r2,c2] = [int(x) for x in re.findall(r"\d+", line)]
        for x in range(r1, r2+1):
            for y in range(c1, c2+1):
                if line.startswith("turn on "):
                    matrix[x][y] += 1
                elif line.startswith("turn off "):
                    matrix[x][y] = max(0, matrix[x][y] - 1)
                elif line.startswith("toggle "):
                    matrix[x][y] += 2
                else:
                    print("Bad instruction")
                    sys.exit(1)
    count = 0
    for x in range(0, 1001):
        for y in range(0, 1001):
            count += matrix[x][y]
    print("Part 2: %d" % count)


if __name__ == "__main__":
    with open("input.txt") as fh:
        data = [line.strip() for line in fh.readlines()]

    part1(data)
    part2(data)
