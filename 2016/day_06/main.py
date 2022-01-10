from collections import Counter

if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip() for l in fh.readlines()]

    chars = [""] * 8

    for l in lines:
        for i, c in enumerate(l):
            chars[i] += c

    msg = []
    altmsg = []
    for s in chars:
        cnt = Counter(s)
        msg.append(cnt.most_common(1)[0][0])
        altmsg.append(cnt.most_common()[-1][0])

    print("Part 1: %s" % "".join(msg))
    print("Part 2: %s" % "".join(altmsg))
