with open("input.txt") as fh:
    numbers = [int(n.rstrip()) for n in fh.readlines()]
frq = 0
for n in numbers:
    frq += n
print("Part 1: %d" % frq)

history = set()
history.add(0)
frq = 0
searching = True
while searching:
    for n in numbers:
        frq += n
        if frq in history:
            print("Part 2: %d" % frq)
            searching = False
            break
        history.add(frq)
