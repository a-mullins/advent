#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 12, Part 2
#   https://adventofcode.com/2015/day/12#part2
import json


def sum_less_red(elem):
    if isinstance(elem, int):
        return elem
    if isinstance(elem, list):
        return sum((sum_less_red(child) for child in elem))
    if isinstance(elem, dict):
        if "red" in elem.values():
            return 0
        else:
            return sum((sum_less_red(child) for child in elem.values()))
    return 0


if __name__ == "__main__":
    top = json.loads(input())
    print(sum_less_red(top))
