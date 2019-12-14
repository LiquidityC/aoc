import sys

class OP:
    direction = ""
    steps = 0

    def __init__(self, direction, steps):
        self.direction = direction
        self.steps = steps

    def __repr__(self):
        return "%s%d" % (self.direction, self.steps)

def matrixCheckPos(matrix, pos, wireNum, steps):
    tpos = (pos[0], pos[1])
    if not tpos in matrix:
        matrix[tpos] = { wireNum: steps }
    else:
        if not wireNum in matrix[tpos]:
            matrix[tpos][wireNum] = steps;

def populateMatrix(matrix, ops, wireNum):
    pos = [0, 0]
    stepCount = 0
    for op in ops:
        if op.direction == "U":
            for i in range(op.steps):
                stepCount += 1;
                pos[0] += 1
                matrixCheckPos(matrix, pos, wireNum, stepCount)
        elif op.direction == "D":
            for i in range(op.steps):
                stepCount += 1;
                pos[0] -= 1
                matrixCheckPos(matrix, pos, wireNum, stepCount)
        elif op.direction == "R":
            for i in range(op.steps):
                stepCount += 1;
                pos[1] += 1
                matrixCheckPos(matrix, pos, wireNum, stepCount)
        elif op.direction == "L":
            for i in range(op.steps):
                stepCount += 1;
                pos[1] -= 1
                matrixCheckPos(matrix, pos, wireNum, stepCount)
        else:
            print("Bad direction:", op)
            sys.exit(1)

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

# Populate a lazy matrix
matrix = { (0, 0): set([1, 2]) }
populateMatrix(matrix, wop1, 1)
populateMatrix(matrix, wop2, 2)


closetsPos = (0, 0)
minDist = sys.maxsize

# Find closest intersection
for k in matrix.keys():
    if k[0] == 0 and k[1] == 0:
        continue
    if 1 in matrix[k] and 2 in matrix[k]:
        if abs(k[0]) + abs(k[1]) < minDist:
            minDist = abs(k[0]) + abs(k[1])
            closestPos = k

print("Closest pos:", closestPos)
print("Min dist: %d" % minDist)

closestWalkPos = (0, 0)
closestWalkDist = sys.maxsize
# Find closest intersection by steps
for k in matrix.keys():
    if k[0] == 0 and k[1] == 0:
        continue
    if 1 in matrix[k] and 2 in matrix[k]:
        if matrix[k][1] + matrix[k][2] < closestWalkDist:
            closestWalkDist = matrix[k][1] + matrix[k][2]
            closestWalkPos = k

print("Closest walk pos:", closestWalkPos)
print("Closest walk dist: %d" % closestWalkDist)
