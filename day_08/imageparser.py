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
    print("Layer count:", len(layers))

    minZeros = 450000
    zeroLayer = []
    for layer in layers:
        zeroCount = layer.count(0)
        if zeroCount < minZeros:
            minZeros = zeroCount
            zeroLayer = layer

    print("Min zeros:", minZeros)
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
        if p != 0:
            print(p, end="")
        else:
            print(" ", end="")
        count += 1
        if count == 25:
            count = 0
            print("")

with open("input.txt") as fh:
    data = fh.readline().rstrip()
layers = part1(data)
part2(layers)
