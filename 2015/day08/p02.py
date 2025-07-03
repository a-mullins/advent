#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 08, Part 2
#   https://adventofcode.com/2015/day/8#part2
from sys import stdin

orig_chars = 0
encoded_chars = 0
table = str.maketrans({'"': r"\"", "\\": "\\\\"})

for line in stdin:
    line = line.strip()
    orig_chars += len(line)
    encoded_chars += len(line.translate(table)) + 2

print(encoded_chars - orig_chars)
