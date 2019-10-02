#!/usr/bin/env python3
from sys import stdin
from itertools import combinations


if __name__ == '__main__':
    containers = []
    for line in stdin:
        containers.append(int(line))

    all_combos = []
    for n in range(1, len(containers)+1):
        all_combos.extend(combinations(containers, n))

    # for tup in all_combos:
    #     print(tup, end='\t\t\t')
    #     print(sum(tup))
    valids = tuple(filter(lambda x: sum(x) == 150, all_combos))
    min_num = min((len(x) for x in valids))
    print(min_num)

    #  for tup in valids[:24]:
    #      print(len(tup))

    print(len(list(filter(lambda x: len(x) == 4, valids))))
