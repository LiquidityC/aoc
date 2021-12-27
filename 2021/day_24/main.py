import sys

class Alu():

    def __init__(self, code):
        self.instructions = code
        self.input_buffer = []
        self.registers = { "x": 0, "y": 0, "w": 0, "z": 0 }

    def __get_val(self, op):
        try:
            return int(op)
        except ValueError:
            return self.registers[op]

    def __read_input(self, loc):
        self.registers[loc] = self.input_buffer.pop(0)

    def __add(self, loc, op):
        a = self.registers[loc]
        b = self.__get_val(op)
        self.registers[loc] = a + b

    def __mul(self, loc, op):
        a = self.registers[loc]
        b = self.__get_val(op)
        self.registers[loc] = a * b

    def __div(self, loc, op):
        a = self.registers[loc]
        b = self.__get_val(op)
        self.registers[loc] = a // b
        if self.registers[loc] < 0:
            self.registers[loc] += 1

    def __mod(self, loc, op):
        a = self.registers[loc]
        b = self.__get_val(op)
        self.registers[loc] = a % b

    def __eql(self, loc, op):
        a = self.registers[loc]
        b = self.__get_val(op)
        if self.registers[loc] == self.__get_val(op):
            self.registers[loc] = 1
        else:
            self.registers[loc] = 0

    def __op(self, code):
        instr, args = code.split(" ", 1)
        if instr == "inp":
            self.__read_input(args)
        elif instr == "add":
            self.__add(*args.split())
        elif instr == "mul":
            self.__mul(*args.split())
        elif instr == "div":
            self.__div(*args.split())
        elif instr == "mod":
            self.__mod(*args.split())
        elif instr == "eql":
            self.__eql(*args.split())
        else:
            print("Bad instruction: %s" % code)
            sys.exit(1)

    def exec(self):
        for code in self.instructions:
            self.__op(code)

if __name__ == "__main__":
    with open("input.txt") as fh:
        ops = [l.strip() for l in fh.readlines()]

    buffer = [
        9,
        9,
        7,
        9,
        9,
        2,
        1,
        2,
        9,
        4,
        9,
        9,
        6,
        7
    ]
    alu = Alu(ops)
    alu.input_buffer = buffer.copy()
    alu.exec()
    assert(alu.registers["z"] == 0)
    print("Part 1: %s" % "".join(str(n) for n in buffer))

    buffer = [
        3,
        4,
        1,
        9,
        8,
        1,
        1,
        1,
        8,
        1,
        6,
        3,
        1,
        1
    ]

    alu.input_buffer = buffer.copy()
    alu.exec()
    assert(alu.registers["z"] == 0)
    print("Part 2: %s" % "".join(str(n) for n in buffer))
