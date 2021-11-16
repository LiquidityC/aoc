def count_valid(triangles):
    count = 0
    for t in triangles:
        tmp = sorted(t)
        if tmp[0] + tmp[1] > tmp[2]:
            count += 1
    return count

if __name__ == "__main__":
    with open("input.txt") as file:
        data = [l.strip() for l in file.readlines()]

    triangles = []
    for l in data:
        triangle = list(map(int, l.split()))
        triangles.append(triangle)

    print("Part 1: %d" % count_valid(triangles))

    chunks = [triangles[x:x+3] for x in range(0, len(triangles), 3)]
    triangles = []
    for c in chunks:
        for i in range(3):
            triangle = []
            for j in range(3):
                triangle.append(c[j][i])
            triangles.append(triangle)

    print("Part 2: %d" % count_valid(triangles))
