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

def run(numbers):
    index = 0
    while numbers[index] != 99:

        op = numbers[index]
        if op == 1:
            numbers = add(numbers, index + 1, index + 2, index + 3)
        elif op == 2:
            numbers = mul(numbers, index + 1, index + 2, index + 3)
        else:
            break;

        index += 4

    return numbers[0]

# Run part 1
numbers = clean_data()
numbers[1] = 12
numbers[2] = 2
print("Operation result: %d" % run(numbers))

# Find the correct input for part 2
for i in range(100):
    for j in range(100):
        numbers = clean_data()
        numbers[1] = i
        numbers[2] = j
        if run(numbers) == 19690720:
            print("Found exec code: %d" % ((i*100) + j))
            sys.exit(0)
