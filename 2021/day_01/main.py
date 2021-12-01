with open("input.txt") as fh:
    numbers = [int(number) for number in fh.readlines()]

current = numbers[0]
count = 0
for n in numbers[1:]:
    if n > current:
        count += 1
    current = n

print("Part 1: %d" % count)

current = numbers[0] + numbers[1] + numbers[2]
sections = [numbers[i:i+3] for i in range(0, len(numbers)-2)]

current = sum(sections[0])
count = 0
for section in sections[1:]:
    if sum(section) > current:
        count += 1
    current = sum(section)

print("Part 2: %d" % count)
