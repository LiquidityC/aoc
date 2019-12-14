import sys

def part1(data, width=25, height=6) :
    layers = []
    layer = []
    count = 0

    for c in data:
        layer.append(int(c))
        count += 1
        if count == 25 * 6:
            count = 0
            layers.append(layer)
            layer = []

    minZeros = 450000
    zeroLayer = []
    for layer in layers:
        zeroCount = layer.count(0)
        if zeroCount < minZeros:
            minZeros = zeroCount
            zeroLayer = layer

    print("Part 1:", zeroLayer.count(1) * zeroLayer.count(2))
    return layers

def part2(layers):
    result = [ 2 for x in range(25*6) ]
    for layer in layers:
        for i in range(len(result)):
            if result[i] == 2:
                result[i] = layer[i]

    count = 0
    for p in result:
        if p == 0:
            print("\033[30;40m%d\033[0m" % p, end="")
        elif p == 1:
            print("\033[37;47m%d\033[0m" % p, end="")
        else:
            print(" ", end="")
        count += 1
        if count == 25:
            count = 0
            print("")
    print("")

with open("input.txt") as fh:
    data = fh.readline().rstrip()
layers = part1(data)
print("")
print("Message:")
part2(layers)
