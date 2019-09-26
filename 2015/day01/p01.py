#!/usr/bin/env python3
floor = 0
for c in input():
    if c == '(':
        floor += 1
    elif c == ')':
        floor -= 1
print(floor)
