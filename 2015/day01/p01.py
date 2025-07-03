#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 01, Part 1
#   https://adventofcode.com/2015/day/1
floor = 0
for c in input():
    if c == "(":
        floor += 1
    elif c == ")":
        floor -= 1
print(floor)
