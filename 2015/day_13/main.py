from itertools import permutations

def best_score(vertices, edges):
    best = 0
    options = permutations(vertices)

    for opt in options:
        score = 0
        current = opt[0]
        for p in opt[1:]:
            score += edges[(current, p)]
            score += edges[(p, current)]
            current = p
        score += edges[(opt[0], opt[-1])]
        score += edges[(opt[-1], opt[0])]
        best = max(score, best)
    return best


if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip()[:-1] for l in fh.readlines()]

    vertices = []
    edges = {}
    for l in lines:
        data = l.split()
        person_a = data[0]
        person_b = data[-1]
        modifier = data[2]
        value = int(data[3])
        if modifier == "lose":
            value *= -1
        edges[(person_a, person_b)] = value
        vertices.append(person_a)
        vertices.append(person_b)

    vertices = set(vertices)

    print("Part 1: %d" % best_score(vertices, edges))

    for p in vertices:
        edges[("You", p)] = 0
        edges[(p, "You")] = 0
    vertices = list(vertices)
    vertices.append("You")
    print("Part 2: %d" % best_score(vertices, edges))
