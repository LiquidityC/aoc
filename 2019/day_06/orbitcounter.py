import sys
import math

def depth_to(orbitmap, root, leaf):
    if root == leaf:
        return 0
    for node in orbitmap[root]:
        if find(orbitmap, node, leaf):
            return 1 + depth_to(orbitmap, node, leaf)

def find(orbitmap, root, leaf):
    if root == leaf:
        return True
    if not root in orbitmap:
        return False
    for node in orbitmap[root]:
        if find(orbitmap, node, leaf):
            return True
    return False

def has_leaves(orbitmap, root, n1, n2):
    return find(orbitmap, root, n1) and find(orbitmap, root, n2)

def find_lowest_common_root(orbitmap, root, n1, n2):
    for node in orbitmap[root]:
        if has_leaves(orbitmap, node, n1, n2):
            return find_lowest_common_root(orbitmap, node, n1, n2)
    return root

def count_orbits(orbitmap, node, depth = 0):
    if not node in orbitmap:
        return depth
    orbits = 0
    for leaf in orbitmap[node]:
        orbits +=  count_orbits(orbitmap, leaf, depth + 1)
    return depth + orbits

with open("input.txt", "r") as fh:
    data = [ s.rstrip() for s in fh.readlines() ]

orbitmap = {}
for orbit in data:
    (node, leaf) = orbit.split(")")
    if not node in orbitmap:
        orbitmap[node] = []
    orbitmap[node].append(leaf)

    
print("Total orbits: %d" % count_orbits(orbitmap, "COM"))

root = find_lowest_common_root(orbitmap, "COM", "YOU", "SAN")
curDepth = depth_to(orbitmap, root, "YOU")
destDepth = depth_to(orbitmap, root, "SAN")
print("Orbit steps for YOU to get to SAN: %d" % (curDepth + destDepth - 2))
