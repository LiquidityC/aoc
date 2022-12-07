tot_space = 70000000
req_space = 30000000

def read_ls(lines, index):
    entries = {}
    size = 0
    while index < len(lines) and lines[index][0] != "$":
        l = lines[index]
        stat, name = l.split()
        if stat.isnumeric():
            size += int(stat)
            entries[name] = int(stat)
        else:
            entries[name] = ({}, 0)
        index += 1
    return entries, size, index

def build_tree(lines, index = 0):
    tree = {}
    assert(lines[index] == "$ ls")
    size = 0
    while index < len(lines):
        l = lines[index]
        cmd = l[2:].split()
        if cmd[0] == "ls":
            entries, subsize, index = read_ls(lines, index+1)
            tree = tree | entries
            size += subsize
        elif cmd[1] != "..":
            # Increment path
            assert(cmd[1] in tree)
            subtree, dirsize, index = build_tree(lines, index+1)
            tree[cmd[1]] = (subtree, dirsize)
            size += dirsize
        else:
            index += 1
            break
    return tree, size, index

def sum_dirs(tree, size):
    sum = 0
    if size < 100000:
        sum += size
    for v in tree.values():
        if not type(v) is int:
            sum += sum_dirs(v[0], v[1])
    return sum

def find_removal_candidate(tree, size, unused_space, best = tot_space):
    if unused_space + size >= req_space:
        best = min(best, size)

    for v in tree.values():
        if not type(v) is int:
            best = find_removal_candidate(v[0], v[1], unused_space, best)
    return best
    

with open("input.txt") as fh:
    lines = [l.strip() for l in fh.readlines()]

tree, size, index = build_tree(lines[1:])

print("Part 1: %d" % sum_dirs(tree, size))
print("Part 2: %d" % (find_removal_candidate(tree, size, tot_space - size)))
