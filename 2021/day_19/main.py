from collections import defaultdict
from itertools import permutations

def orient_coord(orientation, coord):
    remap, negation = orientation
    x = coord[remap[0]] * negation[0]
    y = coord[remap[1]] * negation[1]
    z = coord[remap[2]] * negation[2]

    return (x, y, z)

def coord_diff(coord1, coord2):
    return (coord1[0] - coord2[0], coord1[1] - coord2[1], coord1[2] - coord2[2])

class Beacon():

    def __init__(self, pos, distances=None):
        self.pos = pos
        if not distances:
            self.distances = defaultdict(list)
        else:
            self.distances = distances

    def set_beacon_distance(self, other, num):
        x, y, z = self.pos
        ox, oy, oz = other.pos
        self.distances[num] = (abs(x-ox) + abs(y-oy) + abs(z-oz), other.pos)

    def orient_then_slide(self, orientation, dpos):
        x, y, z = orient_coord(orientation, self.pos)

        dx, dy, dz = dpos
        new_pos = (x + dx, y + dy, z + dz)

        new_distances = {}
        for k, v in self.distances.items():
            dist, coord = v
            bx, by, bz = orient_coord(orientation, coord)
            new_distances[k] = (dist, (bx + dx, by + dy, bz + dz))

        return Beacon(pos=new_pos, distances=new_distances)

    @property
    def distance_set(self):
        return set(val[0] for val in self.distances.values())

    @property
    def distance_pair_set(self):
        return set(pair for pair in self.distances.values())

class Scanner():

    def __init__(self, sid):
        self.sid = sid
        self.beacons = []

    def compare(self, scanner):
        overlaps = {}
        for i, beacon in enumerate(self.beacons):
            for j, other in enumerate(scanner.beacons):
                if len(beacon.distance_set & other.distance_set) >= 12:
                    overlaps[(self.sid, i)] = (scanner.sid, j)
        return overlaps

    def orient_then_slide(self, orientation, pos):
        self.pos = pos

        new_beacons = [
            beacon.orient_then_slide(orientation, pos) for beacon in self.beacons
        ]
        self.beacons = new_beacons

    def get_beacon(self, bid):
        return self.beacons[bid]

    def finalize(self):
        for b1 in self.beacons:
            for num, b2 in enumerate(self.beacons):
                b1.set_beacon_distance(b2, num)

if __name__ == "__main__":
    with open("input.txt") as fh:
        lines = [l.strip() for l in fh.readlines()]


    remaps = [p for p in permutations(range(3), 3)]
    negations = [(1, 1, 1), (-1, -1, 1), (-1, 1, -1), (1, -1, -1)]
    negate_x = [(0, 2, 1), (1, 0, 2), (2, 1, 0)]

    orientations = []
    for remap in remaps:
        for negation in negations:
            negation_ = (
                negation
                if remap not in negate_x
                else (negation[0] * -1, negation[1], negation[2])
            )
            orientations.append((remap, negation_))


    scanner = None
    scanners = []
    for l in lines:
        if not l:
            continue
        if l.startswith("---"):
            scanner = Scanner(len(scanners))
            scanners.append(scanner)
        else:
            scanners[-1].beacons.append(Beacon(tuple(int(n) for n in l.split(","))))

    for s in scanners:
        s.finalize()
    
    matches = defaultdict(set)
    for sid, scanner1 in enumerate(scanners):
        for scanner2 in scanners[sid+1:]:
            found_matches = scanner1.compare(scanner2)
            for b, b_match in found_matches.items():
                matches[b].add(b_match)
                matches[b_match].add(b)
                
    canonical = [0]
    scanners[0].pos = (0, 0, 0)
    scanners[0].orientation = (remaps[0], negations[0])

    iterations = 0
    while len(canonical) < len(scanners) and iterations <= len(scanners):
        for (sid1, bid1), m in matches.items():
            if not sid1 in canonical:
                continue
            b1 = scanners[sid1].get_beacon(bid1)
            for sid2, bid2 in m:
                # If the scanner is already canonical then don't worry about it
                if sid2 in canonical:
                    continue

                b2 = scanners[sid2].get_beacon(bid2)
                for orientation in orientations:
                    # Reorient the second beacon, try to set its coordinates to that of canonical
                    #  and see if its overlapping beacons have the same coordinates relative to canonical
                    #  If they do, then you can apply same transformation to its scanner

                    dpos = coord_diff(b1.pos, orient_coord(orientation, b2.pos))
                    beacon2_ = b2.orient_then_slide(orientation, dpos)
                    if len(b1.distance_pair_set & beacon2_.distance_pair_set) >= 12:
                        canonical.append(sid2)
                        scanners[sid2].orient_then_slide(orientation, dpos)
                        break
        iterations += 1

    canonical_coords = set()
    for scanner in scanners:
        canonical_coords |= {beacon.pos for beacon in scanner.beacons}

    print("Part 1: %d" % len(canonical_coords))

    ans = 0
    for sid, scanner in enumerate(scanners):
        for scanner2 in scanners[sid:]:
            ans = max(ans, sum(map(abs, list(coord_diff(scanner.pos, scanner2.pos)))))

    print("Part 2: %d" % ans)
