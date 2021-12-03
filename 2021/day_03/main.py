def get_common_numbers(data, reverse=False):
    binary = ["0"] * 12
    for i in range(12):
        ones = 0
        zeros = 0
        for l in data:
            if l[i] == "1":
                ones += 1
            else:
                zeros += 1

        if reverse:
            if ones >= zeros:
                binary[i] = "1"
            else:
                binary[i] = "0"
        else:
            if ones >= zeros:
                binary[i] = "0"
            else:
                binary[i] = "1"

    return "".join(binary)

def part1(data):
    binary = get_common_numbers(data)

    gamma = int(binary, 2)
    epsilon = gamma ^ 4095
    print("Part 1: %d" % (gamma * epsilon))

    return "".join(binary)

def calculate_rating(data, reverse = False):
    for index in range(12):
        if len(data) == 1:
            break
        keep_bit = get_common_numbers(data, reverse)[index]
        data = [l for l in data if l[index] == keep_bit]
    return int(data[0], 2)

def part2(data):
    oxygen_data = [l for l in data]
    ox = calculate_rating(data)
    co2 = calculate_rating(data, True)
    print("Part 2: %d" % (ox * co2))

if __name__ == "__main__":
    with open("input.txt") as fh:
        data = [l.strip() for l in fh.readlines()]

    part1(data)
    part2(data)
