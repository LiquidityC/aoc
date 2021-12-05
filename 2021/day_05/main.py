from collections import defaultdict
import math

sign = lambda x: int(math.copysign(1, x))

if __name__ == "__main__":
    with open("input.txt") as fh:
        vectors = []
        for l in fh.readlines():
            l = l.strip()
            start, end = l.split(" -> ")
            [x1, y1] = [int(c) for c in start.split(",")]
            [x2, y2] = [int(c) for c in end.split(",")]
            vectors.append((x1, y1, x2, y2))

    matrix = defaultdict(int)
    for v in vectors:
        [x1, y1, x2, y2] = v
        if x1 == x2:
            step = sign(y2-y1)
            for i in range(y1, y2+step, step):
                matrix[(x1,i)] += 1
        elif y1 == y2:
            step = sign(x2-x1)
            for i in range(x1, x2+step, step):
                matrix[(i,y1)] += 1

    print("Part 1: %d" % sum([1 for v in matrix.values() if v > 1]))

    for v in vectors:
        [x1, y1, x2, y2] = v
        if x1 != x2 and y1 != y2:
            xstep = sign(x2-x1)
            ystep = sign(y2-y1)
            for x, y in zip(range(x1, x2+xstep, xstep), range(y1, y2+ystep, ystep)):
                matrix[(x, y)] += 1

    print("Part 2: %d" % sum([1 for v in matrix.values() if v > 1]))
