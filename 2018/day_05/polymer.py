
with open("input.txt") as fh:
    polymer = [c for c in fh.readline().rstrip()]

while True:
    length = len(polymer)
    i = 0
    while True:
        a, b = polymer[i], polymer[i+1]
        if a.upper() == b or b.upper() == a:
            polymer[i:i+2] = []
            #print("".join(map(str, polymer)))
        i += 1
        if i+1 >= len(polymer):
            break
    if length == len(polymer):
        break

print("Part 1:", len(polymer))
