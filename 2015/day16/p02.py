#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Advent of Code 2015, Day 16, Part 2
#   https://adventofcode.com/2015/day/16#part2
from sys import stdin

target_sue = {
    "children": 3,
    "cats": 7,
    "samoyeds": 2,
    "pomeranians": 3,
    "akitas": 0,
    "vizslas": 0,
    "goldfish": 5,
    "trees": 3,
    "cars": 2,
    "perfumes": 1,
}

if __name__ == "__main__":
    for line in stdin:
        this_sue = {}
        match = True

        for elem in line.partition(":")[2].split(","):
            prop, val = elem.split(":")
            this_sue[prop.strip()] = int(val)

        for val in this_sue.keys():
            if val in ["cats", "trees"]:
                if this_sue[val] <= target_sue[val]:
                    match = False
            elif val in ["pomeranians", "goldfish"]:
                if this_sue[val] >= target_sue[val]:
                    match = False
            else:
                if this_sue[val] != target_sue[val]:
                    match = False

        if match:
            print(line.partition(":")[0])
