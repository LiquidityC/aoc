def cycle(data):
    current = data[0]
    count = 1
    result = ""
    for c in data[1:]:
        if c != current:
            result += "%d%s" % (count, current)
            count = 1
            current = c
        else:
            count += 1
    result += "%d%s" % (count, current)
    return result

if __name__ == "__main__":
    with open("input.txt") as fh:
        data = fh.read()
    first = data
    for _ in range(40):
        first = cycle(first)
    print("Part 1: %d" % len(first))
    second = data
    for _ in range(50):
        second = cycle(second)
    print("Part 2: %d" % len(second))
