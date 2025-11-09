#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 02, Part 2
#   https://adventofcode.com/2015/day/2#part2
from sys import stdin


def ribbon_len(dims):
    dims.sort()
    x, y, z = dims  # x, y are the smallest dimensions
    return 2*x + 2*y + x*y*z  # fmt:skip


lengths = []
for line in stdin:
    lengths.append(ribbon_len([int(x) for x in line.split("x")]))

print(sum(lengths))
