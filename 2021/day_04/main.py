class Board:
    def __init__(self):
        self.rows = []

    def push_row(self, row):
        numbers = [(int(n), False) for n in row.split()]
        if len(numbers) > 5:
            raise RuntimeError("Too many numbers in row")
        self.rows.append(numbers)
        if len(self.rows) > 5:
            print(self.rows)
            raise RuntimeError("Too many rows in board")

    def mark(self, n):
        for i in range(5):
            for j in range(5):
                if self.rows[i][j][0] == n:
                    self.rows[i][j] = (n, True)

    def has_win(self):
        # Check rows
        for i in range(5):
            win = False
            for j in range(5):
                win = self.rows[i][j][1]
                if not win:
                    break
            if win:
                return True

        # Check cols
        for i in range(5):
            win = False
            for j in range(5):
                win = self.rows[j][i][1]
                if not win:
                    break
            if win:
                return True

        return False

    def score(self, draw):
        total = 0
        for i in range(5):
            for j in range(5):
                if not self.rows[i][j][1]:
                    total += self.rows[i][j][0]
        return total * draw

    def __str__(self):
        string = ""
        for r in self.rows:
            for n, c in r:
                if c:
                    string += "\x1b[31m%2s\x1b[0m " % str(n)
                else:
                    string += "\x1b[32m%2s\x1b[0m " % str(n)
            string += "\n"
        return string

def parse_boards(data):
    boards = []
    board = Board()
    for l in data:
        if l == "":
            boards.append(board)
            board = Board()
        else:
            board.push_row(l)
    return boards

def find_winner_board(draws, boards):
    for n in draws:
        for b in boards:
            b.mark(n)
            if b.has_win():
                return b, n
    return None

def find_last_winner(draws, boards):
    winners = []
    for n in draws:
        for b in boards:
            if not b.has_win():
                b.mark(n)
                if b.has_win():
                    winners.append((b, n))

    return winners[-1][0], winners[-1][1]

def part1(draws, boards):
    board, draw = find_winner_board(draws, boards)
    print("Part 1: %d" % board.score(draw))

def part2(draws, boards):
    board, draw = find_last_winner(draws, boards)
    print("Part 2: %d" % board.score(draw))

if __name__ == "__main__":
    with open("input.txt") as fh:
        draws = [int(n) for n in fh.readline().split(",")]
        fh.readline()
        board_data = [l.strip() for l in fh.readlines()]

    part1(draws, parse_boards(board_data))
    part2(draws, parse_boards(board_data))
    
