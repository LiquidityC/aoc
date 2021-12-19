from functools import reduce
from math import ceil, floor
from copy import deepcopy

class SnailNumber():

    def __init__(self, pair, depth = 1):
        self.depth = depth

        left, right = pair;
        if type(left) == SnailNumber or type(left) == int:
            self.left = left
        else:
            self.left = SnailNumber(left, depth + 1)

        if type(right) == SnailNumber or type(right) == int:
            self.right = right
        else:
            self.right = SnailNumber(right, depth + 1)

    def join(self, other):
        self.left = SnailNumber([self.left, self.right], self.depth)
        self.right = SnailNumber([other.left, other.right], self.depth)
        self.left.__increase_depth()
        self.right.__increase_depth()

        while self.explode() or self.split():
            continue

    def __add__(self, other):
        new = SnailNumber([deepcopy(self), deepcopy(other)], self.depth - 1)
        new.__increase_depth()
        return new

    def __increase_depth(self, d = 1):
        self.depth += d
        if type(self.left) == SnailNumber:
            self.left.__increase_depth(d)
        if type(self.right) == SnailNumber:
            self.right.__increase_depth(d)

    def __repr__(self):
        return "[%s,%s]" % (repr(self.left), repr(self.right))

    def __add_left(self, n):
        if type(self.left) == int:
            self.left += n
        else:
            self.left.__add_left(n)

    def __add_right(self, n):
        if type(self.right) == int:
            self.right += n
        else:
            self.right.__add_right(n)

    def explode(self):
        exploding = False

        if self.depth == 5:
            return (self.left, self.right)

        if type(self.left) == SnailNumber:
            exploding = self.left.explode()

        if exploding:
            if self.depth == 4:
                self.left = 0
            if type(self.right) == int:
                self.right += exploding[1]
            else:
                self.right.__add_left(exploding[1])
            return (exploding[0], 0)

        if type(self.right) == SnailNumber:
            exploding = self.right.explode()

        if exploding:
            if self.depth == 4:
                self.right = 0
            if type(self.left) == int:
                self.left += exploding[0]
            else:
                self.left.__add_right(exploding[0])
            return (0, exploding[1])


    def magnitude(self):
        result = 0
        for i, part in enumerate([self.left, self.right]):
            if type(part) == int:
                result += abs(i-3)*part
            else:
                result += abs(i-3)*part.magnitude()
        return result

    def split(self):
        done = False
        if type(self.left) == int:
            if self.left > 9:
                self.left = SnailNumber([int(floor(self.left/2)), int(ceil(self.left/2))], self.depth + 1)
                done = True
        else:
            done = self.left.split()

        if not done:
            if type(self.right) == int: 
                if self.right > 9:
                    self.right = SnailNumber([int(floor(self.right/2)), int(ceil(self.right/2))], self.depth + 1)
                    done = True
            else:
                done = self.right.split()

        return done

def add(n1, n2):
    new = n1.join(n2)
    return n1

if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip() for l in fh.readlines()]

    numbers = []
    for l in lines:
        numbers.append(SnailNumber(eval(l)))

    result = reduce(add, numbers)
    print("Part 1: %d" % result.magnitude())

    maxmag = 0
    for i in range(len(lines)):
        for j in range(len(lines)):
            if i == j:
                continue

            number = SnailNumber(eval(lines[i]))
            number.join(SnailNumber(eval(lines[j])))
            maxmag = max(maxmag, number.magnitude())
            number = SnailNumber(eval(lines[j]))
            number.join(SnailNumber(eval(lines[i])))
            maxmag = max(maxmag, number.magnitude())
    print("Part 2: %d" % maxmag)

