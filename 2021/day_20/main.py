from collections import defaultdict

def neighbors8(x, y):
    modifiers = [
        (-1, -1), (0, -1), (1, -1),
        (-1, 0), (0, 0), (1, 0),
        (-1, 1), (0, 1), (1, 1)
    ]

    for dx, dy in modifiers:
        yield x + dx, y + dy

class Image():

    def __init__(self, iea, image):
        self.cycle = 0
        self.iea = iea
        self.pixels = defaultdict(lambda: 0)
        self.topleft = (0, 0)
        self.width = len(image[0])
        self.height = len(image)
        for y in range(len(image)):
            for x in range(len(image[y])):
                self.pixels[(x, y)] = int(image[y][x] == "#")

    def __get_pixel(self, x, y):
        binary = []
        #print("Checking:", x, y)
        for px, py in neighbors8(x, y):
            #print(px, py)
            if self.pixels[(px, py)]:
                binary.append(1)
            else:
                binary.append(0)
        #print("Binary:", "".join(map(str, binary)))
        idx = int("".join(map(str, binary)), 2)
        #print("(%d, %d) %s" % (x, y, int("".join(map(str, binary)), 2)))
        #print("Returning:", self.iea[idx])
        return int(self.iea[idx] == "#")

    def enhance(self):
        self.cycle += 1
        self.topleft = (self.topleft[0] - 1, self.topleft[1] - 1)
        self.width += 1
        self.height += 1
        new_pixels = {}
        for x in range(self.topleft[0], self.width+1):
            for y in range(self.topleft[1], self.height+1):
                new_pixels[(x, y)] = self.__get_pixel(x, y)

        if self.iea[0] == "#":
            odd = self.cycle % 2
            self.pixels = defaultdict(lambda: odd, new_pixels)
        else:
            self.pixels = defaultdict(lambda: 0, new_pixels)

    def count_lit(self):
        return sum(self.pixels.values())

    def __repr__(self):
        string = ""
        for y in range(self.topleft[1], self.height+1):
            for x in range(self.topleft[0], self.width+1):
                if self.pixels[(x, y)]:
                    string += "#"
                else:
                    string += "."
            string += "\n"
        return string



if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip() for l in fh.readlines()]
        iea = lines[0]
        pic = lines[2:]

    image = Image(iea, pic)
    for _ in range(2):
        image.enhance()
    print("Part 1: %d" % image.count_lit())
    for _ in range(50-2):
        image.enhance()
    print("Part 2: %d" % image.count_lit())
