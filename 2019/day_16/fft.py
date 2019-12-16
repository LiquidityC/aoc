import itertools as it

def duplicate_items(lst, count):
    result = []
    for num in lst:
        result += [num] * count
    return result

# This solution is pretty naive :D
def part1(signal):
    result = signal[:]
    length = len(result)
    for _ in range(100):
        next_signal = []
        for i in range(length):
            pattern = duplicate_items(base_pattern, i+1)
            pattern = pattern[1:] + [pattern[0]]
            ziplst = zip(result, it.cycle(pattern))
            num = abs(sum([i*j for i,j in ziplst])) % 10
            next_signal.append(num)
        result = next_signal
    return result

base_pattern = [ 0, 1, 0, -1 ]
with open("input.txt") as fh:
    signal = [int(x) for x in fh.readline().rstrip()]

result = part1(signal)
nums = signal[:]
length = len(nums)
print("Part 1:", "".join(map(str, result[:8])))

offset = int("".join(map(str, signal[:7])))
assert offset > len(signal)/2

nums = (signal * 10000)[offset:]
length = len(nums)

for _ in range(100):
    for i in range(length-2, -1, -1):
        nums[i] += nums[i+1]
        nums[i] %= 10
print("Part 2:", "".join(map(str, nums[:8])))

