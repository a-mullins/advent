#!/usr/bin/env python3
from sys import stdin


floor = 0
while True:
    c = stdin.read(1)
    if c == '(':
        floor += 1
    elif c == ')':
        floor -= 1
    else:
        break
print(floor)
