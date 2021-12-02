import sys

def part1(moves):
    azimuth = 0
    depth = 0
    for m in moves:
        (motion, count) = m
        if motion == "forward":
            azimuth += count
        elif motion == "down":
            depth += count
        elif motion == "up":
            depth -= count
        else:
            print("Bad move: %s" % [motion, count])
            sys.exit(1)

    print("Part 1: %d" % (depth * azimuth))

def part2(moves):
    azimuth = 0
    depth = 0
    aim = 0
    for m in moves:
        (motion, count) = m
        if motion == "forward":
            azimuth += count
            depth += aim * count
        elif motion == "down":
            aim += count
        elif motion == "up":
            aim -= count
        else:
            print("Bad move: %s" % [motion, count])
            sys.exit(1)

    print("Part 2: %d" % (depth * azimuth))

with open("input.txt") as fh:
    data = [l.strip() for l in fh.readlines()]

moves = [(m, int(c)) for m,c in [l.split() for l in data]]
part1(moves)
part2(moves)
