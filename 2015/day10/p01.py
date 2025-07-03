#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Advent of Code 2015, Day 01, Part 1
#   https://adventofcode.com/2015/day/1
import re


def look_and_say(seq):
    output = ""
    runs = [group[0] for group in re.findall(r"((\d)\2*)", seq)]

    for run in runs:
        output += str(len(run)) + run[0]

    return output


if __name__ == "__main__":
    puzzle = input()
    for _ in range(40):
        puzzle = look_and_say(puzzle)

    print(len(puzzle))
