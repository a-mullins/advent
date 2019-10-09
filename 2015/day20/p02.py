#!/usr/bin/env python3
from array import array

n = 29000000

houses = array('L', (0 for _ in range(n+1)))
for i in range(1, n+1):
    count = 0
    j = i
    while count <= 50 and j < n+1:
        houses[j] += i * 11
        count += 1
        j += i

for house, presents in enumerate(houses):
    if presents >= n:
        print(f'{house} got {presents} presents.')
        break
