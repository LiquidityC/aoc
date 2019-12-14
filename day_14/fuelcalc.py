from collections import defaultdict
import math

def read_conversions(data):
    cookbook = {}
    for line in data:
        ingredients, result = line.rstrip().split(" => ")
        inlst = []
        for ingredient in ingredients.split(", "):
            qty, item = ingredient.split(" ")
            inlst.append((int(qty), item))
        qty, item = result.split()
        cookbook[item] = (int(qty), inlst)
    return cookbook

def undo_reactions(inv, cookbook):
    inventory = defaultdict(lambda: 0, **inv)
    while True:
        available_mats = [m for m in inventory.keys()
                if inventory[m] > 0 and m != "ORE"]

        if not available_mats:
            return inventory["ORE"]
        
        mat = available_mats[0]
        qty = inventory[mat]
        
        run_count = math.ceil(qty / cookbook[mat][0])
        ingredients = cookbook[mat][1]

        inventory[mat] -= run_count * cookbook[mat][0]
        for ingredient in ingredients:
            in_qty, mat = ingredient
            inventory[mat] += in_qty * run_count

def binary_search(fun, low, high):
    while math.ceil(high-low) > 1:
        mid = math.ceil((high+low)/2)
        if fun(mid):
            low, high = mid, high
        else:
            low, high = low, mid
    return mid

def main():
    with open("input.txt") as fh:
        data = fh.readlines()

    cookbook = read_conversions(data)
    result1 = undo_reactions({ "FUEL": 1 }, cookbook)
    print("Part 1:", result1)

    def can_make(x):
        ores = undo_reactions({ "FUEL": x }, cookbook)
        return ores <= 1000000000000
    result2 = binary_search(can_make, 1, 1000000000000)
    print("Part 2:", result2)

if __name__ == "__main__":
    main()
