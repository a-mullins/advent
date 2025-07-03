#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 03, Part 1
#   https://adventofcode.com/2015/day/3
from sys import stdin

d = {}
x = 0
y = 0

d[(0, 0)] = 1

while True:
    c = stdin.read(1)
    if c == ">":
        x += 1
    elif c == "<":
        x -= 1
    elif c == "^":
        y += 1
    elif c == "v":
        y -= 1
    else:
        break

    d[(x, y)] = d.get((x, y), 0) + 1

print(f"presents delivered to {len(d)} houses.")
