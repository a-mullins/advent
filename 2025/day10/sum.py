#!/usr/bin/env python3
from sys import stdin

presses = 0
for line in stdin:
    presses += int(line)
print()
print(presses)
