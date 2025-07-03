#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 08, Part 1
#   https://adventofcode.com/2015/day/8
from sys import stdin

code_chars = 0
mem_chars = 0

for line in stdin:
    line = line.strip()
    code_chars += len(line)
    mem_chars += len(eval(line))

print(code_chars - mem_chars)
