import sys

class NumberEngine:

    def __init__(self, numbers, phaseSetting):
        self.numbers = numbers
        self.index = 0
        self.inputs = [phaseSetting]
        self.immediate = {
                1: False,
                2: False,
                3: False
                }

    def getOutputIndex(self, offset):
        return self.numbers[self.index + offset]

    def getParam(self, offset):
        val = self.numbers[self.index + offset]
        return val if self.immediate[offset] else self.numbers[val]

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
        if len(self.inputs):
            num = self.inputs.pop(0)
        else:
            num = input("sto> ")
        s = self.getOutputIndex(1)
        self.numbers[s] = num
        self.index += 2

    def out(self):
        v1 = self.getParam(1)
        #print("> ", v1)
        self.output = v1
        self.index += 2

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

    def run(self, inputVal):
        self.inputs.append(inputVal)
        while self.numbers[self.index] != 99:

            instr = str(self.numbers[self.index]).zfill(5)
            op = int(instr[-2:])
            self.immediate[1] = bool(int(instr[2]))
            self.immediate[2] = bool(int(instr[1]))
            self.immediate[3] = bool(int(instr[0]))

            if op == 1: # Add
                self.add()
            elif op == 2: # Mul
                self.mul()
            elif op == 3: # Input
                self.sto()
            elif op == 4: # Output
                self.out()
                return False
            elif op == 5: # Jump if true
                self.jit()
            elif op == 6: # Jump if false
                self.jif()
            elif op == 7: # Less than
                self.lt()
            elif op == 8: # Equals
                self.eq()
            else:
                print("[!!] Unknown instruction [%d]:%d"
                        % (self.index, self.numbers[self.index]))
                break
        return True

inputFile = open("input.txt", "r")
data = inputFile.readline()
inputFile.close()
original_input = [int(n) for n in data.rstrip().split(",")]

def clean_data():
    global original_input
    return [n for n in original_input]

def part1(data, settings):
    if len(settings) == 0:
        return data

    result = 0
    for i in range(len(settings)):
        l = list(settings)
        param = l.pop(i)
        eng = NumberEngine(clean_data(), param)
        eng.run(data)
        result = max(result, part1(eng.output, l))
    return result

def getEngines(settings):
    return [
        NumberEngine(clean_data(), settings[0]),
        NumberEngine(clean_data(), settings[1]),
        NumberEngine(clean_data(), settings[2]),
        NumberEngine(clean_data(), settings[3]),
        NumberEngine(clean_data(), settings[4])
        ]

def getSettingCombinations(combinations, combination, settings):
    if len(settings) == 0:
        combinations.append(combination)
        return

    for i in range(len(settings)):
        ts = list(settings)
        tc = list(combination)
        tc.append(ts.pop(i))
        getSettingCombinations(combinations, tc, ts)

def runFeedbackLoop(settings):
    engines = getEngines(settings)
    data = 0
    count = 1
    done = False
    while not done:
        eCount = 1
        for eng in engines:
            done = eng.run(data)
            if done:
                break
            data = eng.output
            eCount += 1
        count += 1
    return data

def part2():
    combinations = []
    getSettingCombinations(combinations, [], [5,6,7,8,9])
    result = 0
    for comb in combinations:
        result = max(result, runFeedbackLoop(comb))
    return result

# Part 1
print("Part 1: ", part1(0, [0, 1, 2, 3, 4]))

# Part 2
print("Part 2: ", part2())
