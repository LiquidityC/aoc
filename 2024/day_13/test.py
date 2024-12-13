file = open("input.txt", "r")

# [A(x,y), B(x,y), X(x,y)]
lines = file.readlines()


machines = []
for i in range(0, len(lines), 4):
    locations = {}
    for line in lines[i : i + 3]:
        label_str, coords_str = line.split(":", 1)
        if label_str.startswith("Button "):
            key = label_str.split()[1]
        else:
            key = "C"
        x, y = coords_str.split(",")
        x, y = x.strip(), y.strip()
        if "+" in x and "+" in y:
            x = int(x.replace("X", ""))
            y = int(y.replace("Y", ""))
        else:
            x = int(x.replace("X=", ""))
            y = int(y.replace("Y=", ""))
        locations[key] = (x, y)
    machines.append(locations)


def count_tokens(moves):
    tokens = 0
    for a, b in moves:
        tokens += a * 3 + b * 1
    print(tokens)


def part1():
    win_moves = []
    for machine in machines:
        # Ax + Bx = X
        # By + Ay = Y
        # 94C1 + 34C2 = 8400
        # 22C1 + 67C2 = 5400
        # 67* (94C1 + 22C2 - 8400) - 22* (34C1 + 67C2 -5400)
        # C1 = (67*8400 - 22*5400)/ (67*94 - 22*34)
        # C1 = (b2*c1 - b1*c2) / (b2*a1 - b1*a2)
        # C2 = (8400 - 94*C1) / 22
        # C2 = (c1 - a1*C1) / b1
        a1 = machine["A"][0]
        a2 = machine["A"][1]
        b1 = machine["B"][0]
        b2 = machine["B"][1]
        c1 = machine["C"][0]
        c2 = machine["C"][1]
        A = (b2 * c1 - b1 * c2) / (b2 * a1 - b1 * a2)
        B = (c1 - a1 * A) / b1
        if A == int(A) and B == int(B):
            win_moves.append((int(A), int(B)))
    count_tokens(win_moves)


def part2():
    win_moves = []
    for machine in machines:
        a1 = machine["A"][0]
        a2 = machine["A"][1]
        b1 = machine["B"][0]
        b2 = machine["B"][1]
        c1 = machine["C"][0] + 10000000000000
        c2 = machine["C"][1] + 10000000000000
        A = (b2 * c1 - b1 * c2) / (b2 * a1 - b1 * a2)
        B = (c1 - a1 * A) / b1
        if A == int(A) and B == int(B):
            win_moves.append((int(A), int(B)))
    count_tokens(win_moves)

part1()
part2()
