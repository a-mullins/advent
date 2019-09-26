#!/usr/bin/env python3
from sys import stdin

floor = 0
pos = 0
while True:
    c = stdin.read(1)
    pos += 1
    if c == '(':
        floor += 1
    elif c == ')':
        floor -= 1
        if floor <= -1:
            break
print(f'position {pos}\tfloor {floor}')
