import sys

class NumberEngine:

    def __init__(self, numbers):
        self.numbers = numbers
        self.index = 0
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
        num = int(input("> ").rstrip())
        s = self.getOutputIndex(1)
        self.numbers[s] = num
        self.index += 2

    def out(self):
        v1 = self.getParam(1)
        print("> %d" % v1)
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

    def run(self):
        self.index = 0
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

inputFile = open("input.txt", "r")
data = inputFile.readline()
inputFile.close()
original_input = [int(n) for n in data.rstrip().split(",")]

def clean_data():
    global original_input
    return [n for n in original_input]

numbers = NumberEngine(clean_data())
numbers.run()
