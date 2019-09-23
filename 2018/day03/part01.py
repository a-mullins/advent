#!/usr/bin/env python
from itertools import chain

# Claim Example:
#   #1 @ 1,3: 4x4
#   <id> @ <x>,<y>: <w>x<h>

SIZE = 1000
fabric = [[0 for _ in range(SIZE)] for _ in range(SIZE)]

with open('input', 'r') as fp:
    for line in fp:
        parts = line.split()
        loc = [int(n) for n in parts[2][:-1].split(',')]
        size = [int(n) for n in parts[-1].split('x')]
        # print(f'Procssing claim {parts[0]}.\n\t'
        #       f'Starts at {loc}, size {size}')
        for i in range(loc[0], loc[0]+size[0]):
            for j in range(loc[1], loc[1]+size[1]):
                # print(f'setting grid loc {i},{j}')
                fabric[j][i] += 1

print(f'Total sq.in. shared by two or more claims: '
      f'{sum([1 for x in chain.from_iterable(fabric) if x > 1])}')
