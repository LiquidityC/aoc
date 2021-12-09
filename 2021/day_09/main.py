from collections import defaultdict
import sys

def get_neighbors(point):
    x, y = point
    return {
        (x+1,y),
        (x-1,y),
        (x,y+1),
        (x,y-1)
    }

def measure_basin(start, area):
    checked = {start}
    neighbors = get_neighbors(start)

    size = 1
    while len(neighbors) > 0:
        n = next(iter(neighbors))
        neighbors.remove(n)
        checked.add(n)
        v = area[n]
        if v < 9:
            size += 1
            new_neighbors = get_neighbors(n)
            neighbors = neighbors | new_neighbors.difference(checked)
    return size


if __name__ == "__main__":
    with open("input.txt") as fh:
        rows = [map(int, list(l.strip())) for l in fh.readlines()]

    area = defaultdict(lambda: defaultdict(lambda: sys.maxsize))
    for y, r in enumerate(rows):
        for x, v in enumerate(r):
            area[(x,y)] = v

    total = 0
    low_points = []
    for y, r in enumerate(rows):
        for x, v in enumerate(r):
            if all(map(lambda h: v < area[h], get_neighbors((x, y)))):
                low_points.append((x, y))
                total += 1 + v

    print("Part 1: %d" % total)
    basin_area = 0
    basin_sizes = [measure_basin(p, area) for p in low_points]
    basin_sizes.sort()
    basin_sizes.reverse()
    answer = 1
    for size in basin_sizes[:3]:
        answer *= size
    print("Part 2: %d" % answer)
