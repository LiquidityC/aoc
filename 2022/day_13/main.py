from functools import cmp_to_key

def compare_list(left :list, right :list):
    for i in range(max(len(left), len(right))):
        if i >= len(left):
            return -1
        elif i >= len(right):
            return 1
        l = left[i]
        r = right[i]
        if isinstance(l, int) and isinstance(r, int):
            if l < r:
                return -1
            elif r < l:
                return 1;
        else:
            result = compare(l, r)
            if result != 0:
                return result
    return 0

def compare(left, right):
    if isinstance(left, list) and isinstance(right, list):
        return compare_list(left, right)
    if isinstance(left, int) and isinstance(right, list):
        return compare_list([left], right)
    if isinstance(left, list) and isinstance(right, int):
        return compare_list(left, [right])
    else:
        return 0

with open("input.txt") as fh:
    data = [l.strip() for l in fh.readlines()]

data = list(filter(lambda x: x != "", data))
pairs = []
while data:
    left, right = data[:2]
    pairs.append((eval(left), eval(right)))
    data = data[2:]

in_order = 0
for i, (l, r) in enumerate(pairs):
    result = compare(l, r)
    assert(result != 0)
    if result == -1:
        in_order += i+1
print("Part 1: %d" % in_order)

packets = []
packets.append([[2]])
packets.append([[6]])
for l, r in pairs:
    packets.append(l);
    packets.append(r);

sorted_packets = sorted(packets, key=cmp_to_key(compare))

indexes = []
for i, p in enumerate(sorted_packets, 1):
    if p == [[2]] or p == [[6]]:
        indexes.append(i)

print("Part 2: %d" % (indexes[0] * indexes[1]))
