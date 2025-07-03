#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Advent of Code 2015, Day 04, Part 2
#   https://adventofcode.com/2015/day/4#part2
from hashlib import md5

secret = input()

n = 0
while True:
    n += 1
    m = md5()
    m.update(bytes(secret + str(n), "utf8"))
    digest = m.hexdigest()
    if digest[:6] == "000000":
        print(f"{secret} + {n}: {digest}")
        break
