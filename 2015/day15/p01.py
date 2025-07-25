#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 15, Part 1
#   https://adventofcode.com/2015/day/15
from sys import stdin
from collections import namedtuple

Ingredient = namedtuple(
    "Properties",
    ["name", "capacity", "durability", "flavor", "texture", "calories"],
)

if __name__ == "__main__":
    ingredients = []
    for line in stdin:
        parts = line.replace(",", "").split()
        props = [int(parts[i]) for i in range(2, 11, 2)]  # 2, 4, .. 10
        # could also do...
        #     props = [int(n) for n in re.findall(r'-?\d+', line)]
        ingred = Ingredient(parts[0].replace(":", ""), *props)
        ingredients.append(ingred)

    combos = []
    for w in range(0, 101):
        for x in range(0, 101 - w):
            for y in range(0, 101 - (w + x)):
                z = 100 - (w + x + y)
                combos.append((w, x, y, z))

    # produce every pair of numbers that can sum to 100.
    scores = []
    for w, x, y, z in combos:
        assert w + x + y + z == 100
        score = 1
        for i in range(1, 5):
            prop = (
                w * ingredients[0][i]
                + x * ingredients[1][i]
                + y * ingredients[2][i]
                + z * ingredients[3][i]
            )
            if prop <= 0:
                score = 0
            else:
                score *= prop

        scores.append(score)

    print(f"Highest val: {max(scores)}")
