from functools import reduce

def priority(char):
    if char.isupper():
        return 27 + ord(char) - ord('A')
    else:
        return 1 + ord(char) - ord('a')

def part1(data):
    score = 0
    for l in data:
        comp1 = set(l[:len(l)//2])
        comp2 = set(l[len(l)//2:])
        score += priority(list(comp1.intersection(comp2))[0])
    print("Part 1: %d" % score)

def part2(data):
    groups = []
    while len(data) > 0:
        groups.append(map(set, list(data[:3])))
        data = data[3:]

    score = 0
    for g in groups:
        score += priority(list(reduce(lambda a, b: a.intersection(b), g))[0])
    print("Part 2: %d" % score)

if __name__ == "__main__":
    with open("input.txt") as fh:
        data = [l.strip() for l in fh.readlines()]

    part1(data)
    part2(data)
