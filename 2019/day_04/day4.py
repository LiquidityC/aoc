def isValid(pw):
    pw = list(str(pw))
    if pw != sorted(pw) or len(set(pw)) == len(pw):
        return False
    return True

def isReallyValid(pw):
    return 2 in [str(pw).count(d) for d in str(pw)]

with open("input.txt") as fd:
    (start, end) = map(lambda x: int(x), fd.readline().rstrip().split("-"))

numbers = range(start, end)
validNums = list(filter(lambda x: isValid(x), numbers))

print("Valid pwds count: %d" % len(validNums))
validNums2 = list(filter(lambda x: isReallyValid(str(x)), validNums))

print("Really valid pwds count: %d" % len(validNums2))
