#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Advent of Code 2015, Day 24, Part 1
#   https://adventofcode.com/2015/day/24
from itertools import combinations
from functools import reduce
import operator
import sys


def div_packages(packages, target_bins, target_mass, parts=None):
    if not parts:
        parts = target_bins
    for run_len in range(1, len(packages)):
        for group in (
            x for x in combinations(packages, run_len) if sum(x) == target_mass
        ):
            if parts == 2:
                return True
            elif parts < target_bins:
                return div_packages(
                    list(set(packages) - set(group)),
                    target_bins,
                    target_mass,
                    parts - 1,
                )
            elif div_packages(
                list(set(packages) - set(group)),
                target_bins,
                target_mass,
                parts - 1,
            ):
                return reduce(operator.mul, group, 1)


if __name__ == "__main__":
    packages = [int(x) for x in sys.stdin]
    bins = int(sys.argv[1])

    print(
        div_packages(
            packages, target_bins=bins, target_mass=sum(packages) / bins
        )
    )
