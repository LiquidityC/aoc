from collections import defaultdict
import sys

class NumberEngine:
    def __init__(self, numbers):
        self.numbers = numbers
        self.index = 0
        self.relativeBase = 0
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
        num = self.inputCommand
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

    def run(self, cmd):
        self.inputCommand = cmd
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

with open("input.txt", "r") as fh:
    data = fh.readline()
original_input = [int(n) for n in data.rstrip().split(",")]
original_input.extend([0 for i in range(3000)])

def clean_data():
    global original_input
    return [n for n in original_input]

NORTH, SOUTH, WEST, EAST = 1, 2, 3, 4

def move(pos, d):
    global NORTH, SOUTH, WEST, EAST
    if d == NORTH:
        return pos[0], pos[1] + 1
    elif d == SOUTH:
        return pos[0], pos[1] - 1
    elif d == EAST:
        return pos[0] + 1, pos[1]
    elif d == WEST:
        return pos[0] - 1, pos[1]

def left_turn(d):
    global NORTH, SOUTH, WEST, EAST
    if d == NORTH:
        return WEST
    elif d == SOUTH:
        return EAST
    elif d == EAST:
        return NORTH
    elif d == WEST:
        return SOUTH

def right_turn(d):
    global NORTH, SOUTH, WEST, EAST
    if d == NORTH:
        return EAST
    elif d == SOUTH:
        return WEST
    elif d == EAST:
        return SOUTH
    elif d == WEST:
        return NORTH

def minimum_steps(maze, dest):
    def fscore(n1, n2):
        return abs(n1[0]-n2[0]) + abs(n1[1]-n2[1])

    directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]

    open_nodes = set()
    scores = defaultdict(lambda: 10000)
    open_nodes.add((0, 0))
    scores[(0, 0)] = 0
    path = {}

    while len(open_nodes):
        node = min(open_nodes, key=lambda pos: fscore(pos, dest))
        if node == dest:
            return scores[node]

        open_nodes.discard(node)
        for direction in directions:
            x, y = node
            dx, dy = direction
            neighbor = x + dx, y + dy
            score = scores[node] + 1
            if maze[neighbor[1]][neighbor[0]] == 0:
                continue
            if scores[neighbor] > score:
                if not neighbor in open_nodes:
                    path[neighbor] = node
                    scores[neighbor] = scores[node] + 1
                    open_nodes.add(neighbor)

def show(maze):
    rows = [pos for pos in maze.keys()]
    rows.sort()
    for y in rows:
        chars = [c for c in maze[y].keys()]
        chars.sort()
        for x in chars:
            val = maze[x][y]
            if x == 0 and y == 0:
                print("0", end="")
            elif val == 0:
                print("#", end="")
            elif val == 1:
                print(".", end="")
            elif val == 2:
                print("X", end="")
        print("")

def flood_time(maze, start):
    directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]
    rooms = set()
    for y in maze:
        for x in maze[y]:
            if maze[y][x] != 0:
                rooms.add((x, y))
    rooms.discard(start)
    neighbors = set()
    neighbors.add(start)
    time = 0
    while len(rooms):
        next_cycle = set()
        for room in neighbors:
            for direction in directions:
                x, y = room
                dx, dy = direction
                neighbor = (x + dx, y + dy)
                if neighbor in rooms:
                    rooms.discard(neighbor)
                    next_cycle.add(neighbor)
        neighbors = next_cycle
        time += 1
    return time



moveDir = WEST
currentPos = (0, 0)
ac_found = False
ac_pos = None
maze = defaultdict(lambda:defaultdict(lambda:0))
maze[0][0] = 1
robot = NumberEngine(clean_data())

while currentPos != (0, 0) or not ac_found:
    left = left_turn(moveDir)
    straight = moveDir
    right = right_turn(moveDir)

    response = robot.run(straight)
    if response > 0:
        currentPos = move(currentPos, straight)
        maze[currentPos[1]][currentPos[0]] = response
        moveDir = left
    else:
        wall = move(currentPos, straight)
        maze[wall[1]][wall[0]] = 0
        response = robot.run(right)
        if response > 0:
            currentPos = move(currentPos, right)
            maze[currentPos[1]][currentPos[0]] = response
        else:
            wall = move(currentPos, straight)
            maze[wall[1]][wall[0]] = 0
            moveDir = right

    if response == 2:
        ac_found = True
        ac_pos = currentPos

print("Part 1:", minimum_steps(maze, ac_pos))
print("Part 2:", flood_time(maze, ac_pos))
