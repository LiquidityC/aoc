from collections import Counter
from collections import defaultdict
from string import ascii_lowercase
import re

def is_valid(name, secid, checksum):
    count = Counter(name.replace("-", ""))
    occurences = defaultdict(list)
    for char, count in count.items():
        occurences[count].append(char)
    order = sorted(occurences.items(), reverse=True)
    validation = ""
    for _,chars in order:
        validation += "".join(sorted(chars))
    return validation.startswith(checksum)

if __name__ == "__main__":
    data = []
    with open("input.txt") as fh:
        for l in fh.readlines():
            m = re.search(r"([\w-]+)-(\d+)\[(\w+)\]", l.strip());
            data.append(m.groups())

    valid = [d for d in data if is_valid(*d)]

    print("Part 1: %d" % sum([int(d[1]) for d in valid]))
            
    codec = ascii_lowercase[:]
    for name, secid, checksum in valid:
        decoded = []
        for c in name:
            if c == '-':
                decoded.append(c)
                continue
            pos = ord(c) - ord('a')
            decoded.append(codec[(pos + int(secid)) % len(codec)])
        name = "".join(decoded)
        if "north" in name:
            print("Part 2: %d" % int(secid))
            break;
