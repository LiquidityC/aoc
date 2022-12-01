from collections import defaultdict
from functools import reduce

def parse_rules(operations):
    bots = defaultdict(dict)

    for op in operations:
        s = op.split()
        if s[0] == "value":
            chip, bot = int(s[1]), int(s[5])
            if not "values" in bots[bot]:
                bots[bot]["values"] = []
            bots[bot]["values"].append(chip)
        else:
            bot, low, high = int(s[1]), int(s[6]), int(s[11])
            lo_dst, hi_dst = s[5], s[10]
            bots[bot]["lo"] = (lo_dst, low)
            bots[bot]["hi"] = (hi_dst, high)
            if not "values" in bots[bot]:
                bots[bot]["values"] = []
    return bots

def step(bots, outputs, b):
    if len(bots[b]["values"]) <= 1:
        return

    vals = bots[b]["values"]
    lo, hi = min(vals), max(vals)
    lo_dst, lo_num = bots[b]["lo"]
    hi_dst, hi_num = bots[b]["hi"]
    if hi == 61 and lo == 17:
        print("Part 1: %d" % b)
    bots[b]["values"] = []

    #print("Bot %d gives %d to %s %d and %d to %s %d" % (b, lo, lo_dst, lo_num, hi, hi_dst, hi_num))
    if lo_dst == "bot":
        bots[lo_num]["values"].append(lo)
        step(bots, outputs, lo_num)
    else:
        outputs[lo_num] = lo
    if (hi_dst == "bot"):
        bots[hi_num]["values"].append(hi)
        step(bots, outputs, hi_num)
    else:
        outputs[hi_num] = hi




if __name__ == "__main__":
    with open("input.txt") as fh:
        operations = [l.strip() for l in fh.readlines()]

    bots = parse_rules(operations)
    starting_bot = 0
    for i,b in bots.items():
        if len(b["values"]) > 1:
            starting_bot = i

    outputs = [0] * 100
    step(bots, outputs, starting_bot)
    print(outputs)
    print("Part 2: %d" % reduce(lambda x, y: x*y, outputs[:3]))
