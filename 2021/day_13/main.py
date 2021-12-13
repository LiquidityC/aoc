#!/usr/bin/env python3

def print_paper(paper):
    for row in paper:
        for col in row:
            if col == 1:
                print("\x1b[31;41m#\x1b[0;0m", end="")
            else:
                print("\x1b[32;42m.\x1b[0;0m", end="")
        print("")

def count_dots(paper):
    total = 0
    for row in paper:
        total += sum(row)
    return total

def fold_x(paper, col):
    for row in paper:
        for i, val in enumerate(row[col+1:]):
            index = col - (i+1)
            row[index] |= val
        del row[col:]

def fold_y(paper, row):
    for i, val in enumerate(paper[row+1:]):
        index = row - (i+1)
        paper[index] = [a | b for a,b in zip(val, paper[index])]
    del paper[row:]

def fold(paper, op):
    axis, val = op
    if axis == "x":
        fold_x(paper, val)
    else:
        fold_y(paper, val)

if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip() for l in fh.readlines()]
        split = lines.index("")
        dots = lines[:split]
        instructions = lines[split+1:]

    points = []
    for dot in dots:
        x, y = dot.split(",")
        points.append((int(x), int(y)))
    
    xmax = max([x for x,_ in points]) + 1
    ymax = max([y for _,y in points]) + 1
    paper = [[0 for _ in range(xmax)] for _ in range(ymax)]

    for x, y in points:
        paper[y][x] = 1

    ops = []
    for i in instructions:
        axis, val = i.split()[2].split("=")
        ops.append((axis, int(val)))

    fold(paper, ops[0])
    print("Part 1: %d" % count_dots(paper))
    for op in ops[1:]:
        fold(paper, op)
    print("Part 2:")
    print_paper(paper)

