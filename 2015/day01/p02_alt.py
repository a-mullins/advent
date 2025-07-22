#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# One-Line Solution for Advent of Code 2015, Day 01, Part 2
#   https://adventofcode.com/2015/day/1#part2
# flake8: noqa
from itertools import accumulate, takewhile;print(list(takewhile(lambda x: x[1] > -2, accumulate(enumerate((1 if c == "(" else -1 for c in input()), start=1), lambda tot, next: (next[0], tot[1] + next[1]))))[-1][0])
