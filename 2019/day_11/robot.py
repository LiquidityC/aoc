import sys
import math
import pprint

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
        s = self.getOutputIndex(1)
        self.numbers[s] = num
        self.index += 2

    def out(self):
        v1 = self.getParam(1)
        self.index += 2
        return int(v1)

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

    def run(self, indata=None):
        if indata != None:
            self.indata.append(indata)

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
        return 99

with open("input.txt", "r") as inputFile:
    data = inputFile.readline()
original_input = [int(n) for n in data.rstrip().split(",")]

# Extend the program with some more memory
original_input.extend([0 for i in range(3000)])

def clean_data():
    global original_input
    return [n for n in original_input]

def runRobot(startColor):
    brain = NumberEngine(clean_data())
    panels = {}
    facingDirections = [
            (0, -1), # UP:0
            (1, 0), # RIGHT:0
            (0, 1), # DOWN:0
            (-1, 0) # LEFT:0
            ]
    facing = 0
    currentPos = (0, 0)
    while True:
        color = startColor
        if currentPos in panels:
            color = panels[currentPos]

        paintColor = brain.run(indata=color)
        if paintColor == 99:
            break
        panels[currentPos] = paintColor

        turn = brain.run()
        if turn == 0:
            facing -= 1
        elif turn == 1:
            facing += 1
        else:
            break
        facing = facing % 4

        direction = facingDirections[facing]
        currentPos = currentPos[0] + direction[0], currentPos[1] + direction[1]
    return panels

panels = runRobot(0)
print("Part 1: {}".format(len(panels)))

panels = runRobot(1)
matrix = [[' '] * 50 for i in range(10)]
for pos, color in panels.items():
    if color:
        matrix[pos[1]][pos[0]] = '#'

print("Part 2:")
for line in matrix:
    print("".join(line))

