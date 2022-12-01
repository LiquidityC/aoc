def decompress1(data):
    length = 0
    while len(data) > 0:
        c = data.pop(0)
        if c == "(":
            index = data.index(")")
            op = "".join(data[:index])
            data = data[index+1:]
            count, repeat = map(int, op.split("x"))
            length += count * repeat
            data = data[count:]
        else:
            length += 1
    return length

def decompress2(data):
    length = 0
    while len(data) > 0:
        c = data.pop(0)
        if c == "(":
            index = data.index(")")
            op = "".join(data[:index])
            data = data[index+1:]
            count, repeat = map(int, op.split("x"))
            if ")" in data:
                length += repeat * decompress2(data[:count])
            else:
                length += count * repeat
            data = data[count:]
        else:
            length += 1
    return length

if __name__ == "__main__":
    with open("input.txt") as fh:
        data = [c for c in fh.readline().strip()]
    print("Part 1: %d" % decompress1(data[:]))
    print("Part 2: %d" % decompress2(data))
