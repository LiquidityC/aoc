import sys

def part1(data):
    azimuth = 0
    depth = 0
    for l in data:
        #print(l, depth, azimuth)
        [motion, count] = l.split()
        if motion == "forward":
            azimuth += int(count)
        elif motion == "down":
            depth += int(count)
        elif motion == "up":
            depth -= int(count)
        else:
            print("Bad move: %s" % [motion, count])
            sys.exit(1)

    print("Part 1: %d" % (depth * azimuth))

def part2(data):
    azimuth = 0
    depth = 0
    aim = 0
    for l in data:
        #print(l, depth, azimuth)
        [motion, count] = l.split()
        if motion == "forward":
            azimuth += int(count)
            depth += aim * int(count)
        elif motion == "down":
            aim += int(count)
        elif motion == "up":
            aim -= int(count)
        else:
            print("Bad move: %s" % [motion, count])
            sys.exit(1)

    print("Part 2: %d" % (depth * azimuth))

with open("input.txt") as fh:
    data = [l.strip() for l in fh.readlines()]

part1(data)
part2(data)
