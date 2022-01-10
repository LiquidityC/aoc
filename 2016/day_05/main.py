from hashlib import md5
import sys

doorid = "wtnhxymk"

if __name__ == "__main__":
    pwd = []
    altpwd = ["-"] * 8
    num = 1
    print("Part 1: ")
    print("Part 2: --------", end="")
    sys.stdout.flush()
    while len(pwd) < 8 or "-" in altpwd:
        hashval = md5((doorid + str(num)).encode())
        num += 1
        hashstr = hashval.hexdigest()
        if hashstr.startswith("00000"):
            if len(pwd) < 8:
                pwd.append(hashstr[5])
            pos = int(hashstr[5], 15)
            if pos < len(altpwd) and altpwd[pos] == "-":
                altpwd[int(hashstr[5], 15)] = hashstr[6]
            print("\x1b[1A", end="")
            print("\r\x1b[2KPart 1: %s" % "".join(pwd))
            print("\r\x1b[2KPart 2: %s" % "".join(altpwd), end="")
            sys.stdout.flush()
    print("\x1b[1A", end="")
    print("\r\x1b[2KPart 1: %s" % "".join(pwd))
    print("\r\x1b[2KPart 2: %s" % "".join(altpwd))
