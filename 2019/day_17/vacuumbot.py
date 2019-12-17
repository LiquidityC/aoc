from collections import defaultdict
import sys

class NumberEngine:

    def __init__(self, numbers):
        self.numbers = numbers
        self.index = 0
        self.relativeBase = 0
        self.indata = []
        self.inputMode = {
                1: 0,
                2: 0,
                3: 0
                }

    def getOutputIndex(self, offset):
        paramMode = self.inputMode[offset]
        param = self.numbers[self.index + offset]
        if paramMode == 2:
            return param + self.relativeBase
        else:
            return param

    def getParam(self, offset):
        val = self.numbers[self.index + offset]

        paramMode = self.inputMode[offset]
        if paramMode == 0:
            return self.numbers[val]
        elif paramMode == 1:
            return val
        elif paramMode == 2:
            return self.numbers[val + self.relativeBase]
        else:
            print("Bad param mode: %d" % paramMode)
            sys.exit(1)

    def add(self):
        v1 = self.getParam(1)
        v2 = self.getParam(2)
        s = self.getOutputIndex(3)
        self.numbers[s] = v1 + v2
        self.index += 4

    def mul(self):
        v1 = self.getParam(1)
        v2 = self.getParam(2)
        s = self.getOutputIndex(3)
        self.numbers[s] = v1 * v2
        self.index += 4

    def sto(self):
        num = self.indata.pop(0)
        print("Reading:", num)
        s = self.getOutputIndex(1)
        self.numbers[s] = num
        self.index += 2

    def out(self):
        v1 = self.getParam(1)
        self.index += 2
        return v1

    def jit(self):
        if bool(self.getParam(1)):
            self.index = self.getParam(2)
        else:
            self.index += 3

    def jif(self):
        if not bool(self.getParam(1)):
            self.index = self.getParam(2)
        else:
            self.index += 3

    def lt(self):
        v1 = self.getParam(1)
        v2 = self.getParam(2)
        s = self.getOutputIndex(3)
        self.numbers[s] = int(v1 < v2)
        self.index += 4

    def eq(self):
        v1 = self.getParam(1)
        v2 = self.getParam(2)
        s = self.getOutputIndex(3)
        self.numbers[s] = int(v1 == v2)
        self.index += 4

    def srb(self):
        v1 = self.getParam(1)
        self.relativeBase += v1
        self.index += 2

    def run(self, in_data=None):
        if in_data:
            self.indata += in_data

        while self.numbers[self.index] != 99:
            instr = str(self.numbers[self.index]).zfill(5)
            op = int(instr[-2:])
            self.inputMode[1] = int(instr[2])
            self.inputMode[2] = int(instr[1])
            self.inputMode[3] = int(instr[0])

            if op == 1: # Add
                self.add()
            elif op == 2: # Mul
                self.mul()
            elif op == 3: # Input
                self.sto()
            elif op == 4: # Output
                return self.out()
            elif op == 5: # Jump if true
                self.jit()
            elif op == 6: # Jump if false
                self.jif()
            elif op == 7: # Less than
                self.lt()
            elif op == 8: # Equals
                self.eq()
            elif op == 9: # Equals
                self.srb()
            else:
                print("[!!] Unknown instruction [%d]:%d"
                        % (self.index, self.numbers[self.index]))
                break
        return None

with open("input.txt", "r") as fh:
    original_input = [int(n) for n in fh.readline().strip().split(",")]

# Extend the program with some more memory
original_input.extend([0 for _ in range(3000)])

def clean_data():
    global original_input
    return [n for n in original_input]

def get_scaffold(robot):
    scaffold = []
    row = []
    while True:
        output = robot.run()
        if output is None:
            break
        if output != 10:
            row.append(str(chr(output)))
        else:
            if len(row):
                scaffold.append(row)
                row = []
            else:
                break
    return scaffold

def get_alignment(scaffold):
    alignment = 0
    for y in range(1, len(scaffold)-2):
        for x in range(1, len(scaffold[y])-2):
            c = scaffold[y][x]
            l, r = scaffold[y][x-1], scaffold[y][x+1]
            u, d = scaffold[y+1][x], scaffold[y-1][x]
            if all([c == "#" for c in [c, l, r, u, d]]):
                alignment += x*y
                scaffold[y][x] = "O"
    return alignment

def get_vacuum_pos(scaffold):
    for y, row in enumerate(scaffold):
        if not "^" in row:
            continue
        for x, c in enumerate(row):
            if c == "^":
                return x, y

def render(scaffold):
    for row in scaffold:
        print("".join(row))

def get_path(nodes, bot):
    direction, start = bot

    path = []
    current_pos = start
    while any(nodes.values()):
        next_pos = current_pos[0] + direction[0], current_pos[1] + direction[1]
        if next_pos in nodes:
            nodes[current_pos] -= 1
            path.append(direction)
            current_pos = next_pos
        else:
            if bool(direction[0]):
                left = current_pos[0], current_pos[1] - 1
                right = current_pos[0], current_pos[1] + 1
                if left in nodes:
                    direction = (0, -1)
                elif right in nodes:
                    direction = (0, 1)
                else:
                    nodes[current_pos] -= 1
            elif bool(direction[1]):
                left = current_pos[0] - 1, current_pos[1]
                right = current_pos[0] + 1, current_pos[1]
                if left in nodes:
                    direction = (-1, 0)
                elif right in nodes:
                    direction = (1, 0)
                else:
                    nodes[current_pos] -= 1
    return path

def get_turn(cur, new):
    if cur[0] > 0:
        return "R" if new[1] > 0 else "L"
    elif cur[0] < 0:
        return "L" if new[1] > 0 else "R"
    elif cur[1] > 0:
        return "L" if new[0] > 0 else "R"
    elif cur[1] < 0:
        return "R" if new[0] > 0 else "L"

def reduce_path(path, direction):
    reduced = []
    steps = 0
    for d in path:
        if d != direction:
            if steps > 0:
                reduced.append(steps+1)
            steps = 0
            reduced.append(get_turn(direction, d))
            direction = d
        else:
            steps += 1
    reduced.append(steps + 1)
    return reduced


numbers = clean_data()
numbers[0] = 2
robot = NumberEngine(numbers)
scaffold = get_scaffold(robot)
print("Part 1:", get_alignment(scaffold))

nodes = defaultdict(int)
for y, row in enumerate(scaffold):
    for x, c in enumerate(row):
        if c != ".":
            nodes[(x, y)] += 1
            if c == "O":
                nodes[(x, y)] += 1

bot = (0, -1), get_vacuum_pos(scaffold)
render(scaffold)
print("Moves:", ",".join(map(str, reduce_path(get_path(nodes, bot), bot[0]))))

# Some manual work has provided the following:
move_routines = [ord(c) for c in "A,A,B,C,B,C,B,C,C,A\n"]
move_a = [ord(c) for c in "R,8,L,4,R,4,R,10,R,8\n"]
move_b = [ord(c) for c in "L,12,L,12,R,8,R,8\n"]
move_c = [ord(c) for c in "R,10,R,4,R,4\n"]
no = [ord(c) for c in "n\n"]

robot.run(move_routines + move_a + move_b + move_c + no)
while True:
    response = robot.run()
    if response is None:
        break
    print(str(chr(response)), response)
