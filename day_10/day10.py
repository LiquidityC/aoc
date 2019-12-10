
class Asteroid :

    def __init__(self, pos, dim):
        self.pos = pos
        self.dim = dim
        self.los = {}

    def __repr__(self):
        return "(%d, %d, %d, %d)" % (self.pos[0], self.pos[1], self.dim[0], self.dim[1])

def getAsteroidsFromLines(lines):
    x = 0
    y = 0
    asteroids = []
    for line in lines:
        for c in line:
            if not c == "#":
                continue
            asteroids.append(Asteroid((x+4, y+4), (2, 2)))
            x += 10
        y += 10
    return (10+len(lines[0])*10, 10+len(lines)*10), asteroids

def traceRay(asteroids, mapDim, vector):
    # This should return the first asteroid the vector hits
    pass

with open("input.txt", "r") as fh:
    (mapDim, asteroids) = getAsteroidsFromLines(fh.readlines())

for a in asteroids:
    for b in asteroids:
        if b.pos in a.los:
            continue
        obj = traceRay(asteroids, mapDim, (a.pos, b.pos))
        if obj and not obj.pos in a.los:
            a.los[obj.pos] = obj
            obj.los[a.pos] = a

print(mapDim)
