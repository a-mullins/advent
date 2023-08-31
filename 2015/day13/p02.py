#!/usr/bin/env python3
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
