#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 19, Part 1
#   https://adventofcode.com/2015/day/19
import re
from sys import stdin

replacements = []
for line in stdin:
    if "=>" in line:
        parts = line.split()
        replacements.append((parts[0], parts[-1]))
    elif line != "":
        medicine_molecule = line.strip()

results = set()
for target, replacement in replacements:
    for m in re.finditer(target, medicine_molecule):
        results.add(
            medicine_molecule[: m.start()]
            + replacement
            + medicine_molecule[m.end() :]
        )
print(len(results))
