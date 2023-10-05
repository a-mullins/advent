#!/usr/bin/env python3
from sys import stdin
from itertools import combinations


if __name__ == "__main__":
    containers = []
    for line in stdin:
        containers.append(int(line))

    all_combos = []
    for n in range(1, len(containers) + 1):
        all_combos.extend(combinations(containers, n))

    valids = [x for x in all_combos if sum(x) == 150]
    min_num = min((len(x) for x in valids))
    print(min_num)

    print(len([x for x in valids if len(x) == 4]))
