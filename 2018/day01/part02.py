#!/usr/bin/env python
import sys
from itertools import cycle

with open(sys.argv[1], 'rt') as fp:
    input_lines = fp.readlines()

acc = 0
freqs = set()
freqs.add(acc)
for line in cycle(input_lines):
    acc += int(line)
    if acc in freqs:
        break
    else:
        freqs.add(acc)
print(acc)
