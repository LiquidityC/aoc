from itertools import count

def gcd(a, b):
    while (b > 0):
        a, b = b, a % b
    return a

def lcm(a, b):
    return a * b / gcd(a, b)

def read_moon(line):
    data = line[1:-1]
    values = data.split(", ")
    return [int(values[0].split("=")[1]),
            int(values[1].split("=")[1]),
            int(values[2].split("=")[1])], [0, 0, 0]

def apply_gravity(m1, m2):
    m1pos, m1vel = m1
    m2pos, m2vel = m2

    for i in range(3):
        if m1pos[i] > m2pos[i]:
            m1vel[i] -= 1
            m2vel[i] += 1
        elif m1pos[i] < m2pos[i]:
            m1vel[i] += 1
            m2vel[i] -= 1

def apply_velocity(moon):
    pos, vel = moon
    for i in range(3):
        pos[i] += vel[i]

def calculate_energies(moons):
    energies = []
    for moon in moons:
        pos, vel = moon
        pot = sum([abs(x) for x in pos])
        kin = sum([abs(x) for x in vel])
        energies.append((pot, kin))
    return energies

def run_step(moons):
    for i in range(len(moons)-1):
        for moon in moons[i+1:]:
            apply_gravity(moons[i], moon)

    for moon in moons:
        apply_velocity(moon)

def get_moons(inputlines):
    moons = []
    for line in inputlines:
        moons.append(read_moon(line))
    return moons

with open("input.txt") as fh:
    inputlines = [ line.rstrip() for line in fh.readlines() ]

moons = get_moons(inputlines)
for _ in range(1000):
    run_step(moons)

# Summarize the data
energies = calculate_energies(moons)
print("Part 1: {}".format(sum([x*y for x, y in energies])))

# Counting the number of steps it takes for every moon to return
# to one of their coordinate origins with velocity for that coordinate == 0
# The time it takes for all moons to return to their original start position
# should be the 'least common multiple' of those step counts
moons = get_moons(inputlines)
initial = [moon for moon in moons]

stepCounts = [None, None, None]
for step in count(start=1):
    run_step(moons)
    for d in range(3):
        if stepCounts[d] is not None:
            continue
        for m in range(len(moons)):
            if moons[m][0][d] != initial[m][0][d] or moons[m][1][d] != 0:
                break
        else:
            stepCounts[d] = step

    if all(stepCounts):
        break

# The reason for the '* 2' is because our algorithm actually only finds the
# steps to do half a rotation first. Eg. y == 0 when x == 1 or x == -1 (with
# the unit circle in mind)
print("Part 2: {}".format(int(lcm(lcm(stepCounts[0], stepCounts[1]), stepCounts[2]) * 2)))
