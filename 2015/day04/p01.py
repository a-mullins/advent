#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 04, Part 1
#   https://adventofcode.com/2015/day/4
from hashlib import md5

secret = input()

n = 0
while True:
    n += 1
    m = md5()
    m.update(bytes(secret + str(n), "utf8"))
    digest = m.hexdigest()
    if digest[:5] == "00000":
        print(f"{secret} + {n}: {digest}")
        break
