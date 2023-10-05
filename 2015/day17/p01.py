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

    print(len([tup for tup in all_combos if sum(tup) == 150]))
