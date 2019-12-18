from collections import defaultdict

STATE_CACHE = {}

def get_keys_from(maze, start):
    if start in STATE_CACHE:
        return STATE_CACHE[start]

    directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]

    open_nodes = {}
    scores = defaultdict(lambda: 10000000)
    scores[start] = 0
    open_nodes[start] = set()
    keys = {}

    while len(open_nodes):
        node = next(iter(open_nodes))
        blockers = open_nodes[node]
        del open_nodes[node]
        for direction in directions:
            blockers_cpy = blockers.copy()
            x, y = node
            dx, dy = direction
            neighbor = x + dx, y + dy
            score = scores[node] + 1
            next_node = maze[neighbor[1]][neighbor[0]]
            if next_node == "#":
                continue
            if next_node.isupper():
                blockers_cpy.add(next_node)
            if next_node.islower():
                keys[next_node] = (neighbor, min(score, scores[neighbor]), blockers_cpy)
            if scores[neighbor] > score:
                if not neighbor in open_nodes:
                    scores[neighbor] = score
                    open_nodes[neighbor] = blockers_cpy
    STATE_CACHE[start] = keys
    return keys

def get_start_pos(maze):
    for y, row in enumerate(maze):
        for x, c in enumerate(row):
            if c == "@":
                return (x, y)

PATHING_CACHE = {}
def get_best_pathing(maze, start, collected=set()):
    keys = get_keys_from(maze, start)
    cache_key = (tuple(collected), start)
    if cache_key in PATHING_CACHE:
        return PATHING_CACHE[cache_key]

    if len(collected) == len(keys.keys()):
        return 0

    min_steps = 10000
    for key, data in keys.items():
        tmpcollected = collected.copy()
        if key in tmpcollected:
            continue

        tmpcollected.add(key)
        pos, steps, doors = data
        if len(doors.difference([k.upper() for k in tmpcollected])):
            continue
        result =  get_best_pathing(maze, pos, tmpcollected)
        min_steps = min(min_steps, steps + result)

    PATHING_CACHE[cache_key] = min_steps
    return min_steps

def main():
    with open("input.txt") as fh:
        maze = [l.strip() for l in fh.readlines()]

    start_pos = get_start_pos(maze)
    print("Part 1:", get_best_pathing(maze, start_pos))

if __name__ == "__main__":
    main()
