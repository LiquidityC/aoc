
def get(matrix, x, y):
    try:
        r = matrix[y]
    except IndexError:
        return None

def increase_all(matrix):
    for i in range(10):
        for j in range(10):
            matrix[i][j] += 1

def get_overloaded(matrix):
    overloaded = []
    for i in range(10):
        for j in range(10):
            if matrix[i][j] > 9:
                overloaded.append((i, j))
    return set(overloaded)


def flash(matrix, i, j):
    for di in range(i-1, i+2):
        for dj in range(j-1, j+2):
            if di >= 0 and di < 10 and dj >= 0 and dj < 10:
                matrix[di][dj] += 1

def flash_all(matrix):
    flashed = set()
    overloaded = get_overloaded(matrix)
    while len(overloaded) > 0:
        i, j = next(iter(overloaded))
        if matrix[i][j] > 9:
            flashed.add((i, j))
            flash(matrix, i, j)
            overloaded.update(get_overloaded(matrix))
            overloaded  = overloaded.difference(flashed)
    for i, j in flashed:
        matrix[i][j] = 0

    return len(flashed)


if __name__ == "__main__":
    with open("input.txt") as fh:
        matrix = []
        for l in fh.readlines():
            matrix.append([int(n) for n in l.strip()])


    flash_count = 0
    for i in range(100):
        increase_all(matrix)
        flash_count += flash_all(matrix)

    print("Part 1: %d" % flash_count)

    count = 100
    while True:
        count += 1
        increase_all(matrix)
        if flash_all(matrix) == 100:
            break
    print("Part 2: %d" % count)
