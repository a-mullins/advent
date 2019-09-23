#!/usr/bin/env python3
from sys import stdin


def ribbon(dims):
    dims.sort()
    x, y, z = dims  # x, y are the smallest dimensions
    return 2*x + 2*y + x*y*z


packages = [line.strip().split('x') for line in stdin.readlines()]
print(sum([ribbon([int(x) for x in package]) for package in packages]))
