#!/usr/bin/env python3
from sys import stdin


def paper(dims):
    dims.sort()
    x, y, z = dims  # x, y are the smallest dimensions
    return 3*x*y + 2*y*z + 2*z*x  # fmt:skip


packages = [line.strip().split("x") for line in stdin.readlines()]
print(sum(paper([int(x) for x in package]) for package in packages))
