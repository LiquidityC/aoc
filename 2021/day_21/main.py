from collections import Counter
from functools import lru_cache
from itertools import product

class Player():

    def __init__(self, start_pos, score = 0):
        self.pos = start_pos - 1
        self.score = score

    def move(self, n):
        self.pos = (self.pos + n) % 10
        self.score += self.pos + 1

    def clone_move(self, n):
        npos = (self.pos + n) % 10
        nscore = self.score + self.pos + 1
        return Player(npos, nscore)

def deterministic_dice_rolls():
    n = 1
    while True:
        yield ([n, n+1, n+2], [n+3, n+4, n+5])
        n = (n + 6 - 1) % 100 + 1

def part1(s1, s2):
    player1 = Player(s1)
    player2 = Player(s2)

    rolls = 0
    for r1, r2 in deterministic_dice_rolls():
        player1.move(sum(r1))
        player2.move(sum(r2))
        rolls += 6
        if player1.score > 1000 or player2.score > 1000:
            break
    min_score = min(player1.score, player2.score)
    print("Part 1: %d" % (min_score * rolls))

rolls = Counter([sum(t) for t in product([1, 2, 3], repeat=3)])
@lru_cache(maxsize=None)
def play(starting_positions, starting_scores, player):

    wins = [0, 0]
    for roll, universes in rolls.items():
        positions = list(starting_positions)
        scores = list(starting_scores)

        positions[player] = (positions[player] + roll - 1) % 10 + 1
        scores[player] += positions[player]

        if scores[player] >= 21:
            wins[player] += universes
        else:
            subwins = play(tuple(positions), tuple(scores), (player+1)%2)
            for i in range(2):
                wins[i] += subwins[i]*universes
    return wins

def part2(s1, s2):
    wins = [0, 0]
    wins = play((s1, s2), (0, 0), 0)
    print("Part 2: %d" % max(wins))

if __name__ == "__main__":
    with open("input.txt") as fh:
        s1 = int(fh.readline().split(": ")[1])
        s2 = int(fh.readline().split(": ")[1])

    part1(s1, s2)
    part2(s1, s2)
