def parse_literal(index, binary):
    payload = ""
    done = False
    while not done:
        payload += "".join(binary[index+1:index+5])
        done = binary[index] == "0"
        index += 5

    return index, int(payload, 2)

def parse_operator(index, binary):
    done = False
    subpackets = []
    if binary[index] == "0":
        index += 1
        bitlen = int("".join(binary[index:index+15]), 2)
        index += 15
        end = bitlen + index
        while index < end:
            index, packet = parse_packet(index, binary)
            subpackets.append(packet)
        return index, subpackets
    else:
        index += 1
        packlen = int("".join(binary[index:index+11]), 2)
        index += 11
        packcount = 0
        while packcount < packlen:
            index, packet = parse_packet(index, binary)
            subpackets.append(packet)
            packcount += 1
        return index, subpackets

def parse_packet(index, binary):
    ver = int("".join(binary[index:index+3]), 2)
    index += 3
    tid = int("".join(binary[index:index+3]), 2)
    index += 3
    packet = None
    if tid == 4:
        index, payload = parse_literal(index, binary)
        packet = (ver, tid, payload)
    else:
        index, payload = parse_operator(index, binary)
        packet = (ver, tid, payload)
    return index, packet

def parse_packets(binary):
    index = 0
    packets = []
    while index < len(binary)-3:
        index, packet = parse_packet(index, binary)
        packets.append(packet)
    return packets

def version_sum(packets):
    versum = 0
    for v, t, p in packets:
        if t == 4:
            versum += int(v)
        else:
            versum += int(v)
            versum += version_sum(p)
    return versum

def value(packet):
    v, t, p = packet
    if t == 4:
        return p
    elif t == 0:
        return sum(map(value, p))
    elif t == 1:
        result = 1
        for n in map(value, p):
            result *= n
        return result
    elif t == 2:
        return min(map(value, p))
    elif t == 3:
        return max(map(value, p))
    elif t == 5:
        v1, v2 = map(value, p)
        if v1 > v2:
            return 1
        else:
            return 0
    elif t == 6:
        v1, v2 = map(value, p)
        if v1 < v2:
            return 1
        else:
            return 0
    elif t == 7:
        v1, v2 = map(value, p)
        if v1 == v2:
            return 1
        else:
            return 0

if __name__ == "__main__":
    with open("input.txt") as fh:
        hexstring = fh.read().strip()
        binary = []
        for c in hexstring:
            binstr = format(int(c, 16), "04b")
            binary.extend(binstr)

    packets = parse_packets(binary)
    print("Part 1: %d" % version_sum(packets))
    print("Part 2: %d" % value(packets[0]))
