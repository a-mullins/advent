#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Alternative Solution for Advent of Code 2015, Day 01, Part 1
#   https://adventofcode.com/2015/day/1

print(sum((1 if c == "(" else -1 for c in input())))
