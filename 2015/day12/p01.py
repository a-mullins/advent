#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Advent of Code 2015, Day 12, Part 1
#   https://adventofcode.com/2015/day/12
import re

print(sum((int(s) for s in re.findall(r"-*\d+", input()))))
