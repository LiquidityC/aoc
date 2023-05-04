import re

def mdist(ax, ay, bx, by):
    return abs(ax - bx) + abs(ay - by)

def search(sensors):
    multiplier = 4000000
    for sensor, distance in sensors.items():
        sx, sy = sensor
        for x in range(max(0,sx-distance-1), min(multiplier,sx+distance+2)):
            for y in [sy+(distance+1-abs(x-sx)), sy-(distance+1-abs(x-sx))]:
                if y in range(multiplier+1):
                    for s,d in sensors.items():
                        if mdist(x,y,*s) <= d:
                            break
                    else:
                        return x * multiplier + y

if __name__ == "__main__":
    with open("input.txt") as fh:
        data = [l.strip() for l in fh.readlines()]
    coordinates = []
    for l in data:
        coordinates.append((int(n) for n in re.findall(r"\b\d+\b", l)))
    row_coverage = set()
    beacons = set()
    sensors = {}
    limit = 2000000

    for sx, sy, bx, by in coordinates:
        if (by == limit):
            beacons.add(bx)
        dist = mdist(sx, sy, bx, by)
        sensors[(sx, sy)] = dist
        if limit in range(sy-dist, sy+dist+1):
            row_coverage |= set(range(sx - dist + abs(limit-sy), sx + dist - abs(limit-sy)+1))

    row_coverage -= beacons
    print("Part 1: %d" % len(row_coverage))
    print("Part 2: %d" % search(sensors))
