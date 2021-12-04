def increment_pwd(pwd, index = None):
    if index == None:
        index = len(pwd) - 1

    char = pwd[index]
    char = chr(ord(char) + 1)
    if char == chr(ord("z")+1):
        char = "a"
        pwd = increment_pwd(pwd, index-1)
    pwd[index] = char
    return pwd

def check_serial(pwd):
    tripples = [pwd[i:i+3] for i in range(len(pwd)-2)]
    for x,y,z in tripples:
        if ord(x) + 1 == ord(y) == ord(z) - 1:
            return True
    return False

def check_doubles(pwd):
    count = 0
    chars = []
    doubles = [pwd[i:i+2] for i in range(len(pwd)-1)]
    for a,b in doubles:
        if a == b:
            count += 1
            chars.append(a)
    return count == 2 and chars[0] != chars[1]

def check_illegals(pwd):
    return not any(map(lambda c: c in pwd, ["i", "l", "o"]))

if __name__ == "__main__":
    with open("input.txt") as fh:
        data = fh.readline().strip()

    pwd = [c for c in data]
    while True:
        pwd = increment_pwd(pwd)
        if check_serial(pwd) and check_doubles(pwd) and check_illegals(pwd):
            break
    print("Part 1: %s" % "".join(pwd))
    while True:
        pwd = increment_pwd(pwd)
        if check_serial(pwd) and check_doubles(pwd) and check_illegals(pwd):
            break
    print("Part 2: %s" % "".join(pwd))
