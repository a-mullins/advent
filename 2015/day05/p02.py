#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 05, Part 2
#   https://adventofcode.com/2015/day/5#part2
from sys import stdin
import re


def is_nice(string):
    rule1 = re.search(r"(..).*\1", string)
    rule2 = re.search(r"(.).\1", string)
    return bool(rule1 and rule2)


print(sum((1 for line in stdin.readlines() if is_nice(line))))
