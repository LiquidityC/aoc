def part1(data):
    code_len = 0
    eval_len = 0
    for c, e in data:
        code_len += len(c)
        eval_len += len(e)
    print("Part 1: %d" % (code_len - eval_len))

def part2(data):
    escape_len = 0
    code_len = 0
    for c, e in data:
        code_len += len(c)
        escaped = c.translate(str.maketrans({
            "\\": "\\\\",
            "\"": "\\\""
        }))
        escape_len += len("\"" + escaped + "\"")
    print("Part 2: %d" % (escape_len - code_len))
    

with open("input.txt") as fh:
    data = [(l.strip(), eval(l.strip())) for l in fh.readlines()]

part1(data)
part2(data)
