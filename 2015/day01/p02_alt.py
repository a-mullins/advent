#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Advent of Code 2015, Day 01, Part 2, Alternative One-Line Sol'n
#   https://adventofcode.com/2015/day/1#part2
# flake8: noqa
from collections import deque;from itertools import accumulate, count, takewhile;print(deque(takewhile(lambda x: x[0] > -2, accumulate(zip((1 if c == '(' else -1 for c in input()), count(start=1)), lambda tot, next: (tot[0] + next[0], next[1]))), maxlen=1)[0][1])
