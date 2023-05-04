def map_cave(lines: list[str]):
    cave = {}
    coords = []
    for l in lines:
        coords.append(list(map(lambda t: (int(t[0]), int(t[1])), [tuple(c.split(",")) for c in l.split(" -> ")])))

    for w in coords:
        pos = w[0]
        cave[pos] = True
        ox, oy = pos
        for c in w[1:]:
            dx, dy = c
            if dx != ox:
                for x in range(min(dx, ox), max(dx, ox)+1):
                    cave[(x,oy)] = True
            if dy != oy:
                for y in range(min(dy, oy), max(dy, oy)+1):
                    cave[(ox,y)] = True
            ox, oy = dx, dy

    return cave

def trickle_sand_without_floor(cave, ox, oy):
    while oy < 500:
        if not (ox, oy+1) in cave:
            oy += 1
        elif not (ox-1, oy+1) in cave:
            ox -= 1
            oy += 1
        elif not (ox+1, oy+1) in cave:
            ox += 1
            oy += 1
        else:
            cave[(ox, oy)] = True
            return True
    return False

def trickle_sand_with_floor(cave, ox, oy, floor):
    px, py = ox, oy
    while not (ox, oy) in cave:
        if py == floor - 1:
            cave[(px, py)] = True
            return True
        elif not (px, py+1) in cave:
            py += 1
        elif not (px-1, py+1) in cave:
            px -= 1
            py += 1
        elif not (px+1, py+1) in cave:
            px += 1
            py += 1
        else:
            cave[(px, py)] = True
            return True
    return False

with open("input.txt") as fh:
    lines = [l.strip() for l in fh.readlines()]

cave = map_cave(lines)
count = 0
while (trickle_sand_without_floor(cave, 500, 0)):
    count += 1
print("Part 1: %d" % count)

cave = map_cave(lines)
floor = max([k[1] for k in cave.keys()]) + 2
count = 0
while trickle_sand_with_floor(cave, 500, 0, floor):
    count += 1
print("Part 2: %d" % count)

