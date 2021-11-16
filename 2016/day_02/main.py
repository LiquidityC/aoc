import io

keypad = [
    [ 0, 0, 0, 0, 0 ],
    [ 0, 1, 2, 3, 0 ],
    [ 0, 4, 5, 6, 0 ],
    [ 0, 7, 8, 9, 0 ],
    [ 0, 0, 0, 0, 0 ],
]

actual_keypad = [
    [ 0, 0, 0, 0, 0, 0, 0 ],
    [ 0, 0, 0, 1, 0, 0, 0 ],
    [ 0, 0, 2, 3, 4, 0, 0 ],
    [ 0, 5, 6, 7, 8, 9, 0 ],
    [ 0, 0, 'A', 'B', 'C', 0, 0 ],
    [ 0, 0, 0, 'D', 0, 0, 0 ],
    [ 0, 0, 0, 0, 0, 0, 0 ],
]

def parse_line(line, pos, pad):
    #print(pos, line)
    for c in line:
        orig_pos = pos
        if c == 'U':
            pos = (pos[0], pos[1] - 1)
        elif c == 'D':
            pos = (pos[0], pos[1] + 1)
        elif c == 'L':
            pos = (pos[0] - 1, pos[1])
        elif c == 'R':
            pos = (pos[0] + 1, pos[1])
        #print("%s -> %s" % (c, pos))
        if pad[pos[1]][pos[0]] == 0:
            pos = orig_pos
    #print("%s -> %s" % (pos, keypad[pos[1]][pos[0]]))
    return pos

def part1(lines):
    passcode = []
    pos = (1, 1)
    for l in lines:
        pos = parse_line(l, pos, keypad)
        passcode.append(keypad[pos[1]][pos[0]])
    print("Part 1:", "".join([str(n) for n in passcode]))

def part2(lines):
    passcode = []
    pos = (1, 3)
    for l in lines:
        pos = parse_line(l, pos, actual_keypad)
        passcode.append(actual_keypad[pos[1]][pos[0]])
    print("Part 2:", "".join([str(n) for n in passcode]))

if __name__ == "__main__":
    with io.open("input.txt", "r") as fh:
        lines = list(map(str.strip, fh.readlines()))
    part1(lines)
    part2(lines)

