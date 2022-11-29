from itertools import tee

def has_abba(sequences):
    for seq in sequences:
        for i in range(len(seq) - 3):
            a, b, c, d = seq[i:i+4]
            if a != b and a == d and b == c:
                print(a, b, c, d)
                return True
    return False

def get_aba(sequences):
    abas = set()
    for seq in sequences:
        for i in range(len(seq) - 2):
            a, b, c = seq[i:i+3]
            if a != b and a == c:
                abas.add((a, b))
    return abas

def has_any_bab(abas, sequences):
    for seq in sequences:
        for i in range(len(seq) - 2):
            a, b, c = seq[i:i+3]
            if a == c and a != b:
                if (b, a) in abas:
                    return True
    return False


if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip() for l in fh.readlines()]

    first_count = 0
    second_count = 0
    for l in lines:
        split = l.replace("[", " ").replace("]", " ").split()
        external = split[0::2]
        internal = split[1::2]
        if has_abba(external) and not has_abba(internal):
            first_count += 1
        abas = get_aba(external)
        if has_any_bab(abas, internal):
            second_count += 1
    print("Part 1: %d" % first_count)
    print("Part 2: %d" % second_count)
