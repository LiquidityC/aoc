import io

def read_input():
    with io.open("input.txt", "r") as fh:
        return fh.read()

directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]
def part1(data):
    commands = [c.strip() for c in data.split(", ")]
    facing = 0
    pos = (0, 0)
    visited = {}
    visited[pos] = True
    actual_pos = None
    for cmd in commands:
        turn, steps = cmd[0], int(cmd[1:])
        if turn == "L":
            facing = (facing - 1) % len(directions)
        else:
            facing = (facing + 1) % len(directions)
        x, y = directions[facing]
        for _ in range(steps):
            pos = (pos[0] + x, pos[1] + y)
            if pos in visited and not actual_pos:
                actual_pos = pos
            visited[pos] = True
    print("Part 1: %d" % (abs(pos[0]) + abs(pos[1])))
    print("Part 2: %d" % (abs(actual_pos[0]) + abs(actual_pos[1])))


if __name__ == "__main__":
    data = read_input()
    part1(data)
