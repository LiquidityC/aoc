wires = {}

def value_of(v):
    try:
        return int(v)
    except ValueError:
        return wires[v]

def parse_expression(ex):
    data = ex.strip().split()
    if len(data) == 1:
        return value_of(data[0])
    if len(data) == 2:
        n = value_of(data[1])
        return ~n
    a, e, b = data
    a, b = value_of(a), value_of(b)
    if e == 'AND':
        return a & b
    if e == 'OR':
        return a | b
    if e == 'LSHIFT':
        return a << b
    if e == 'RSHIFT':
        return a >> b

def evaluate(line):
    ex, n = line.split(' -> ')
    wires[n] = parse_expression(ex)

with open("input.txt") as fh:
    data = [l.strip() for l in fh.readlines()]

while data:
    for i, d in enumerate(data):
        try:
            evaluate(d)
            del data[i]
        except KeyError:
            continue

print(wires["a"])
