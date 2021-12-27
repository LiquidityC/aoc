def next_step(p, c, h, w):
    if c == ">":
        return ((p[0] + 1) % w, p[1])
    else:
        return (p[0], (p[1] + 1) % h)

def move(grid, t, h, w):
    updates = {}
    for k,v in grid.items():
        if v == t:
            n = next_step(k, t, h, w)
            if not n in grid:
                updates[k] = n
    for s,d in updates.items():
        grid[d] = t
        del grid[s]
    return len(updates) > 0

if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip() for l in fh.readlines()]

    grid = {}
    for y, l in enumerate(lines):
        for x, c in enumerate(l):
            if c == ">" or c == "v":
                grid[(x, y)] = c

    height, width = len(lines), len(lines[0])

    steps = 0
    while True:
        steps += 1
        hmove = move(grid, ">", height, width)
        vmove = move(grid, "v", height, width)
        if not hmove and not vmove:
            break
    print("Part 1: %d" % steps)
