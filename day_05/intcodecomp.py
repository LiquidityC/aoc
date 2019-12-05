import sys

inputFile = open("input.txt", "r")
data = inputFile.readline()
inputFile.close()

original_input = [int(n) for n in data.rstrip().split(",")]

def clean_data():
    global original_input
    return [n for n in original_input]

def add(numbers, i1, i2, ri):
    dest = numbers[ri]
    p1 = numbers[i1]
    p2 = numbers[i2]
    numbers[dest] = numbers[p1] + numbers[p2]
    return numbers;

def mul(numbers, i1, i2, ri):
    dest = numbers[ri]
    p1 = numbers[i1]
    p2 = numbers[i2]
    numbers[dest] = numbers[p1] * numbers[p2]
    return numbers;

def sto(numbers, i):
    num = input("> ")
    numbers[numbers[i]] = int(num)
    return numbers

def out(numbers, i):
    print("[**] %d" % numbers[numbers[i]])
    return numbers

def run(numbers):
    index = 0
    while numbers[index] != 99:

        op = numbers[index]
        if op == 1:
            numbers = add(numbers, index + 1, index + 2, index + 3)
            index += 4
        elif op == 2:
            numbers = mul(numbers, index + 1, index + 2, index + 3)
            index += 4
        elif op == 3:
            numbers = sto(numbers, index + 1)
            index += 2
        elif op == 4:
            numbers = out(numbers, index + 1)
            index += 2
        elif op >= 1000:
            (numbers, shift) = param(numbers, index)
            index += shift
        else:
            break;

# Run part 1
numbers = clean_data()
run(numbers)
