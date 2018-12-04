#!/usr/bin/env python
from itertools import combinations

with open('input', 'r') as fp:
    lines = fp.readlines()

for line1, line2 in combinations(lines, 2):
    matches = [t[0] == t[1] for t in zip(line1.strip(), line2.strip())]
    if matches.count(False) == 1:
        print(''.join([t[0] for t in zip(line1, matches) if t[1]]))
        break
