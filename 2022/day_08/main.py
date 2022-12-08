with open("input.txt") as fh:
    forest = [list(map(int, l.strip())) for l in fh.readlines()]

def count_visible_trees(forest):
    visible = set()

    # Scan each row
    for y,l in enumerate(forest):

        # From left
        height = -1
        for x,t in enumerate(l):
            if t > height:
                visible.add((x, y))
                height = t

        # From right
        height = -1
        for x,t in reversed(list(enumerate(l))):
            if t > height:
                visible.add((x, y))
                height = t

    # Sweep scan rows from top
    sweep = list(forest[0])
    for x in range(len(sweep)):
        visible.add((x, 0))
    for y,l in enumerate(forest[1:],1):
        for x,t in enumerate(sweep):
            if l[x] > t:
                sweep[x] = l[x]
                visible.add((x, y))

    # Sweep scan rows from bottom
    sweep = list(forest[-1])
    for x in range(len(sweep)):
        visible.add((x, len(forest)-1))
    for y,l in reversed(list(enumerate(forest[:-1]))):
        for x,t in enumerate(sweep):
            if l[x] > sweep[x]:
                sweep[x] = l[x]
                visible.add((x, y))

    return len(visible)

def scenic_score(x, y, w, h):
    directions = [(1,0), (0,1), (-1,0), (0,-1)]
    total = 1
    for d in directions:
        dx, dy = x, y
        score = 0
        level = forest[y][x]
        while True:
            dx += d[0]
            dy += d[1]
            if not dx in range(w) or not dy in range(h):
                break
            score += 1
            if forest[dy][dx] >= level:
                break
        total *= score
    return total

def find_best_tree(forest):
    best = 0
    width = len(forest[0])
    height = len(forest)
    for y in range(height):
        for x in range(width):
            best = max(best, scenic_score(x, y, width, height))
    return best


print("Part 1: %d" % count_visible_trees(forest))
print("Part 2: %d" % find_best_tree(forest))
