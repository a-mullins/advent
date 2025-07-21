#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 01, Part 2
#   https://adventofcode.com/2015/day/1#part2
from sys import stdin

floor = 0
pos = 0
while True:
    c = stdin.read(1)
    pos += 1
    if c == "(":
        floor += 1
    elif c == ")":
        floor -= 1
        if floor <= -1:
            break
print(f"position {pos}\tfloor {floor}")
