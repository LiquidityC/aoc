from collections import Counter

def get_next_steps(edges, current):
    options = set()
    for n1, n2 in edges:
        if n1 == current:
            options.add(n2)
        elif n2 == current:
            options.add(n1)
    return options

def path_valid(path):
    counts = Counter(path)
    for n, count in counts.items():
        if n.islower() and count > 1:
            return False

    return True

def alternate_valid(path):
    counts = Counter(path)
    small_visits = 0
    if counts['start'] > 1 or counts['end'] > 1:
        return False
    for n, count in counts.items():
        if n.islower() and count > 2:
            return False
        if n.islower() and count == 2:
            small_visits += 1
            if small_visits > 1:
                return False
    return True

def find_all_paths(edges, start, end, validator):
    routes = [[start]]
    paths = []
    while len(routes) > 0:
        path = routes.pop(0)
        for opt in get_next_steps(edges, path[-1]):
            new_path = path.copy()
            new_path.append(opt)
            if opt == end:
                paths.append(new_path)
            elif validator(new_path):
                routes.append(new_path)
    return paths

if __name__ == "__main__":
    with open("input.txt") as fh:
        edges = []
        for l in fh.readlines():
            (start, end) = l.strip().split("-")
            edges.append((start, end))

    print("Part 1: %d" % len(find_all_paths(edges, 'start', 'end', path_valid)))
    print("Part 2: %d" % len(find_all_paths(edges, 'start', 'end', alternate_valid)))
