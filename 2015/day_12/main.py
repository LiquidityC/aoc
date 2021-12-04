import json

def find_sum(obj):
    sum = 0
    if type(obj) == dict:
        for k,v in obj.items():
            sum += find_sum(v)
    elif type(obj) == list:
        for v in obj:
            sum += find_sum(v)
    elif type(obj) == int:
        sum += obj
    return sum

def find_sum_no_red(obj):
    sum = 0
    if type(obj) == dict:
        if not "red" in obj.values():
            for k,v in obj.items():
                sum += find_sum_no_red(v)
    elif type(obj) == list:
        for v in obj:
            sum += find_sum_no_red(v)
    elif type(obj) == int:
        sum += obj
    return sum

with open("input.txt") as fh:
    data = json.loads(fh.read())

print("Part 1: %d" % find_sum(data))
print("Part 2: %d" % find_sum_no_red(data))
