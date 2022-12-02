from enum import Enum

class Choice(Enum):
    ROCK = 1
    PAPER = 2
    SCISSORS = 3

class Result(Enum):
    LOSS = 0
    DRAW = 1
    WIN = 2

def play1(a, b):
    score = b.value
    if a == b:
        score += 3
    elif a == Choice.ROCK:
        if b == Choice.PAPER:
            score += 6
    elif a == Choice.PAPER:
        if b == Choice.SCISSORS:
            score += 6
    elif a == Choice.SCISSORS:
        if b == Choice.ROCK:
            score += 6
    return score

def play2(a, result):
    score = result.value * 3
    if result == Result.DRAW:
        score += a.value
    elif result == Result.WIN:
        if a == Choice.ROCK:
            score += Choice.PAPER.value
        elif a == Choice.PAPER:
            score += Choice.SCISSORS.value
        elif a == Choice.SCISSORS:
            score += Choice.ROCK.value
    elif result == Result.LOSS:
        if a == Choice.ROCK:
            score += Choice.SCISSORS.value
        elif a == Choice.PAPER:
            score += Choice.ROCK.value
        elif a == Choice.SCISSORS:
            score += Choice.PAPER.value
    return score

if __name__ == "__main__":
    with open("input.txt") as fh:
        data = [l.strip() for l in fh.readlines()]

    score1 = 0
    score2 = 0
    for l in data:
        split = l.split()
        a = ord(split[0]) - (ord("A") - 1)
        b = ord(split[1]) - (ord("X") - 1)
        score1 += play1(Choice(a), Choice(b))
        score2 += play2(Choice(a), Result(b-1))
    print("Part 1: %d" % score1)
    print("Part 2: %d" % score2)
