#!/usr/bin/env python3
from sys import stdin
from itertools import permutations


def table_hapiness(order: list, relations: dict):
    assert len(order) >= 3

    acc = 0
    for i in range(len(order)):
        cur_rels = relations[order[i]]

        # if first elem...
        if i == 0:
            prev_rel_val = cur_rels[order[-1]]
            next_rel_val = cur_rels[order[i + 1]]
        # if last elem...
        elif i + 1 == len(order):
            prev_rel_val = cur_rels[order[i - 1]]
            next_rel_val = cur_rels[order[0]]
        else:
            prev_rel_val = cur_rels[order[i - 1]]
            next_rel_val = cur_rels[order[i + 1]]
        acc += prev_rel_val + next_rel_val
    return acc


def parse_relations(lines):
    # eg: {'Alice': {'Bob':54, 'Carol':-79},
    #      'Bob': {'Alice': 83, 'Carol': -7}}
    relations = {}
    for line in lines:
        parts = line.split()
        cur = parts[0]
        target = parts[-1][:-1]
        if "gain" == parts[2]:
            happiness_mod = int(parts[3])
        else:
            happiness_mod = -1 * int(parts[3])
        relations.setdefault(cur, {})[target] = happiness_mod
    return relations


if __name__ == "__main__":
    rels = parse_relations(stdin.readlines())
    people = rels.keys()

    print(
        max((table_hapiness(permut, rels) for permut in permutations(people)))
    )
