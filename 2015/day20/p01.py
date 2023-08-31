#!/usr/bin/env python3
from array import array

n = 29000000

houses = array("L", (0 for _ in range(n + 1)))
for i in range(1, n + 1):
    for j in range(i, n + 1, i):
        houses[j] += i * 10

for house, presents in enumerate(houses):
    if presents >= n:
        print(f"{house} got {presents} presents.")
        break
