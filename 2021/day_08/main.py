from collections import Counter

if __name__ == "__main__":
    with open("input.txt") as fh:
        data = []
        for l in fh.readlines():
            sections = l.split(" | ")
            data.append((list(sections[0].split()), list(sections[1].split())))

    count = 0
    for d in data:
        counts = Counter(map(len, d[1]))
        count += counts.get(2, 0)
        count += counts.get(3, 0)
        count += counts.get(4, 0)
        count += counts.get(7, 0)

    print("Part 1: %d" % count)

    total = 0
    for d in data:
        patterns = [None] * 10

        signals = set(d[0])
        digits = d[1]
        for sig in signals:
            signal = set(sig)
            if len(signal) == 2:
                patterns[1] = signal
            elif len(signal) == 4:
                patterns[4] = signal
            elif len(signal) == 3:
                patterns[7] = signal
            elif len(signal) == 7:
                patterns[8] = signal

        for sig in signals:
            signal = set(sig)
            if len(signal) == 6:
                if (patterns[7] | patterns[4]).issubset(signal):
                    patterns[9] = signal
                elif patterns[1].issubset(signal):
                    patterns[0] = signal
                else:
                    patterns[6] = signal
            elif len(signal) == 5:
                if patterns[1].issubset(signal):
                    patterns[3] = signal
                elif len(signal.difference(patterns[7] | patterns[4])) == 1:
                    patterns[5] = signal
                else:
                    patterns[2] = signal

        output = ""
        for d in digits:
            for v, signal in enumerate(patterns):
                if set(d) == signal:
                    output += str(v)
                    break

        total += int(output)

    print("Part 2: %d" % total)


