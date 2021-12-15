from collections import defaultdict
from math import inf as INFINITY
import heapq

def neighbors(x, y, h, w):
    for dx, dy in ((-1, 0), (1, 0), (0, -1), (0, 1)):
        nx, ny = (x + dx, y + dy)
        if 0 <= nx < w and 0 <= ny < h:
            yield nx, ny

def f_score(cave, pos, goal):
    x, y = pos;
    gx, gy = goal
    return (gx-x) + (gy-y) + cave[y][x]

def find_path(cave):
    queue = [ (0, (0, 0)) ]
    score = defaultdict(lambda: INFINITY, { (0, 0): 0 })
    goal = (len(cave[0])-1, len(cave)-1)
    height, width = (len(cave), len(cave[0]))
    visited = set()

    while queue:
        dist, current = heapq.heappop(queue)

        if current == goal:
            return dist
        if current in visited:
            continue

        visited.add(current)
        cx, cy = current

        for n in neighbors(cx, cy, height, width):
            nx, ny = n
            nscore = cave[ny][nx] + dist
            if nscore < score[n]:
                score[n] = nscore
                heapq.heappush(queue, (nscore, n))

    return INFINITY

def incr(val, incr):
    if val+incr > 9:
        return val+incr-9
    else:
        return val+incr

def expand_cave(cave):
    # First elongate the rows
    ncave = []
    for r in cave:
        nr = r.copy()
        for i in range(1,5):
            nr += [incr(n, i) for n in r]
        ncave.append(nr)
    # Then repeat the rows
    for i in range(1,5):
        for j in range(len(cave)):
            ncave.append([incr(n, i) for n in ncave[j]])
    return ncave


if __name__ == "__main__":
    with open("input.txt") as fh:
        cave = []
        for l in fh.readlines():
            cave.append([int(i) for i in l.strip()])
    print("Part 1: %d" % find_path(cave))
    ncave = expand_cave(cave)
    print("Part 2: %d" % find_path(ncave))

