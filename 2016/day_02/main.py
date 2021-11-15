import io

keypad = [
    [ 1, 2, 3 ],
    [ 4, 5, 6 ],
    [ 7, 8, 9 ],
]

def parse_line(line, pos):
    print(pos, line)
    for c in line:
        if c == 'U':
            pos = (pos[0], max(pos[1] - 1, 0))
        elif c == 'D':
            pos = (pos[0], min(pos[1] + 1, 2))
        elif c == 'L':
            pos = (max(pos[0] - 1, 0), pos[1])
        elif c == 'R':
            pos = (min(pos[0] + 1, 2), pos[1])
    return pos

def part1(lines):
    passcode = []
    pos = (1, 1)
    for l in lines:
        pos = parse_line(l, pos)
        passcode.append(keypad[pos[0]][pos[1]])
    print("Part 1:", "".join([str(n) for n in passcode]))

if __name__ == "__main__":
    with io.open("input.txt", "r") as fh:
        lines = list(map(str.strip, fh.readlines()))
    part1(lines)

