def find_marker(signal, mlen):
    marker = list(signal[:mlen])
    index = mlen
    while len(set(marker)) != mlen:
        marker.pop(0)
        marker.append(signal[index])
        index += 1
    return index

with open("input.txt") as fh:
    signal = fh.readline().strip()

print("Part 1: %d" % find_marker(signal, 4))
print("Part 2: %d" % find_marker(signal, 14))

