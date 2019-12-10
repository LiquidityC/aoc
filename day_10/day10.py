import math
import pprint

def countAsteroidsInLOS(station, asteroids):
    detected = set()
    for asteroid in asteroids:
        if asteroid == station:
            continue
        dx, dy = asteroid[0]-station[0], asteroid[1]-station[1]
        g = abs(math.gcd(dx, dy))
        detected.add((dx//g, dy//g))
    return detected


with open("input.txt") as fh:
    data = [s.strip() for s in fh.readlines()]

asteroids = set()
for y in range(len(data)):
    for x in range(len(data[y])):
        if data[y][x] == "#":
            asteroids.add((x, y))

counts = []
for station in asteroids:
    inLOS = countAsteroidsInLOS(station, asteroids)
    counts.append((len(inLOS), station, inLOS))
counts.sort(reverse=True)
amtInLOS, station, inLOS = counts[0]
print("Part 1: {} {}".format(station, amtInLOS))

# This assumes asteroids in LOS > 200 otherwise we'd have to rotate more

# Explanation:
# The inversion of atan2 arguments orients the "unit circle" making the
# y axis go from left to right and the x axis from top to bottom
# the highest radian values will then be the ones straight above the source
# Example: atan2(0, -1) = PI (Rule: atan2(y, x) <= PI)
destroyed = [(math.atan2(dx, dy), (dx, dy)) for dx, dy in inLOS]
destroyed.sort(reverse=True)
dx, dy = destroyed[199][1]
x, y = station[0]+dx, station[1]+dy
while (x, y) not in asteroids:
    x, y = x+dx, y+dy

print("Part 2: {} {}".format((x, y), x*100 + y))
