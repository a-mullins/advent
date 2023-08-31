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

    valids = tuple(filter(lambda x: sum(x) == 150, all_combos))
    min_num = min((len(x) for x in valids))
    print(min_num)

    print(len(list(filter(lambda x: len(x) == 4, valids))))
