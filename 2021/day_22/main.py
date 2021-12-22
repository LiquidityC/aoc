from collections import defaultdict

def volume(rx, ry, rz):
    return (rx[1]-rx[0]+1) * (ry[1]-ry[0]+1) * (rz[1]-rz[0]+1)

def intersects(c1, c2):
    x1, y1, z1 = c1
    x2, y2, z2 = c2
    xs = max(x1[0], x2[0])
    ys = max(y1[0], y2[0])
    zs = max(z1[0], z2[0])
    xe = min(x1[1], x2[1])
    ye = min(y1[1], y2[1])
    ze = min(z1[1], z2[1])

    if xs <= xe and ys <= ye and zs <= ze:
        return ((xs, xe), (ys, ye), (zs, ze))

def reboot(ops):
    reactor = defaultdict(int)
    for state, new in ops:
        for old, sgn in reactor.copy().items():
            intersection = intersects(old, new)
            if intersection:
                reactor[intersection] -= sgn

        if state > 0:
            reactor[new] = 1

    return sum([volume(*c)*v for c, v in reactor.items()])

if __name__ == "__main__":
    ops = []
    with open("input.txt") as fh:
        for l in fh.readlines():
            op, cube = l.strip().split()
            strranges = cube.split(",")

            ranges = []
            for r in strranges:
                start, end = r[2:].split("..")
                ranges.append((int(start), int(end)))

            ops.append((int(op == "on"), tuple(ranges)))

    print("Part 1: %d" % reboot(ops[:20]))
    print("Part 2: %d" % reboot(ops))
