from itertools import count

if __name__ == "__main__":

    with open("input.txt") as fh:
        splits = fh.read().strip().split()
        x_range = splits[2].split("=")[1][:-1]
        y_range = splits[3].split("=")[1]
        min_x, max_x = (int(n) for n in x_range.split(".."))
        min_y, max_y = (int(n) for n in y_range.split(".."))

    best, count = 0, 0
    for tx in range(0, max_x+1):
        for ty in range(min_y, -min_y):
            x, y, h = 0, 0, 0
            vx, vy = tx, ty
            while y > min_y:
                x, y, h = x + vx, y + vy, max(h, y)
                vx, vy = max(0, vx-1), vy - 1
                if min_x <= x <= max_x and min_y <= y <= max_y:
                    count += 1
                    best = max(best, h)
                    break

    print("Part 1: %d" % best)
    print("Part 2: %d" % count)

