import sys

class OP:
    direction = ""
    steps = 0

    def __init__(self, direction, steps):
        self.direction = direction
        self.steps = steps

    def __repr__(self):
        return "%s%d" % (self.direction, self.steps)

def matrixCheckPos(matrix, pos, wireNum):
    if not pos[0] in matrix:
        matrix[pos[0]] = {}
    if not pos[1] in matrix[pos[0]]:
        matrix[pos[0]][pos[1]] = (".", set(wireNum))
    elif pos[1] in matrix[pos[0]]:
        entry = matrix[pos[0]][pos[1]]
        if not wireNum in entry[1]:
            matrix[pos[0]][pos[1]] = ("x", set(wireNum))
        elif wireNum in entry[1]:
            matrix[pos[0]][pos[1]][1].add(wireNum)

data1 = ""
data2 = ""
with open("input.txt", "r") as inputFile:
    data1 = inputFile.readline().rstrip()
    data2 = inputFile.readline().rstrip()

path1 = [op for op in data1.split(",")]
path2 = [op for op in data2.split(",")]

wop1 = []
for ostr in path1:
    wop1.append(OP(ostr[:1], int(ostr[1:])))

wop2 = []
for ostr in path2:
    wop2.append(OP(ostr[:1], int(ostr[1:])))

matrix = { 0: { 0: "."}}
pos = [0, 0]
for op in wop1:
    if op.direction == "U":
        for i in range(op.steps+1):
            pos[0] += 1
            matrixCheckPos(matrix, pos, 1)
    elif op.direction == "D":
        for i in range(op.steps+1):
            pos[0] -= 1
            matrixCheckPos(matrix, pos, 1)
    elif op.direction == "R":
        for i in range(op.steps+1):
            pos[1] += 1
            matrixCheckPos(matrix, pos, 1)
    elif op.direction == "L":
        for i in range(op.steps+1):
            pos[1] -= 1
            matrixCheckPos(matrix, pos, 1)
    else:
        print("Bad direction:", op)
        sys.exit(1)

