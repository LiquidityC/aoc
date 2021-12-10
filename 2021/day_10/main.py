from collections import defaultdict
import sys
import math

def get_id(c):
    increment = 1
    if c in "])>}":
        increment = -1

    if c in "()":
        return "par", increment
    elif c in "[]":
        return "bra", increment
    elif c in "{}":
        return "squ", increment
    elif c in "<>":
        return "ang", increment
    else:
        print("Invalid char: %s" % c)
        sys.exit(1)

def check_line(l):
    current, increment = get_id(l[0])
    depth = defaultdict(int)
    order = []
    depth[current] += increment
    order.append(current)
    if depth[current] < 0:
        False, current
    for c in l[1:]:
        current, increment = get_id(c)
        depth[current] += increment
        if increment < 0: 
            if order[-1] != current:
                return False, c
            else:
                order.pop(-1)
        else:
            order.append(current)

        depth[current] += increment
        if depth[current] < 0:
            False, c

    score = 0
    order.reverse()
    for c in order:
        score *= 5
        if c == "par":
            score += 1
        elif c == "bra":
            score += 2
        elif c == "squ":
            score += 3
        elif c == "ang":
            score += 4

    return True, score


if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip() for l in fh.readlines()]

    error_score = 0
    scores = []
    for i, l in enumerate(lines):
        ok, result = check_line(l)
        if not ok:
            if result == ")":
                error_score += 3
            elif result == "]":
                error_score += 57
            elif result == "}":
                error_score += 1197
            elif result == ">":
                error_score += 25137
        else:
            scores.append(result)
    print("Part 1: %d" % error_score)
    scores.sort()
    print("Part 2: %d" % scores[int(len(scores)/2)])

