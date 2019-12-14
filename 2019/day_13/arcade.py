import sys

class NumberEngine:

    def __init__(self, numbers):
        self.numbers = numbers
        self.index = 0
        self.relativeBase = 0
        self.inputData = 0
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
        num = int(self.inputData)
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

    def run(self, inputData=None):
        self.inputData = inputData

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

def get_joypos(ball, paddle):
    if not ball or not paddle:
        return 0
    if ball[0] < paddle[0]:
        return -1
    elif ball[0] > paddle[0]:
        return 1
    return 0

def play_game(numbers):
    tiles = set()
    ball = None
    paddle = None
    tile = []
    joystick = 0
    score = 0
    while True:
        tile.append(numbers.run(inputData=joystick))
        if None in tile:
            break
        if len(tile) == 3:
            if tile[2] == 3: #Paddle
                paddle = tuple(tile)
                joystick = get_joypos(ball, paddle)
            elif tile[2] == 4: # Ball
                ball = tuple(tile)
                joystick = get_joypos(ball, paddle)
            elif tile[0] == -1: #Score
                score = tile[2]
            else:
                tiles.add(tuple(tile))
            tile = []
    return (tiles, ball, paddle, score)

inputFile = open("input.txt", "r")
data = inputFile.readline()
inputFile.close()
original_input = [int(n) for n in data.rstrip().split(",")]

# Extend the program with some more memory
original_input.extend([0 for i in range(3000)])

def clean_data():
    global original_input
    return [n for n in original_input]

# Part 1
numbers = NumberEngine(clean_data())
gameState = play_game(numbers)
count = 0
for tile in gameState[0]:
    if tile[2] == 2:
        count += 1
print("Part 1: {}".format(count))

# Part 2
program = clean_data()
program[0] = 2
numbers = NumberEngine(program)
tiles, ball, paddle, score = play_game(numbers)
print("Part 2: {}".format(score))
