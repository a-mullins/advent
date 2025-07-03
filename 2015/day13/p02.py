#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Advent of Code 2015, Day 13, Part 2
#   https://adventofcode.com/2015/day/13#part2
from p01 import parse_relations, table_hapiness
from itertools import permutations
from sys import stdin

if __name__ == "__main__":
    rels = parse_relations(stdin.readlines())
    rels["Me"] = {}
    for key in rels.keys():
        rels[key]["Me"] = 0
        rels["Me"][key] = 0
    people = rels.keys()

    print(
        max((table_hapiness(permut, rels) for permut in permutations(people)))
    )
