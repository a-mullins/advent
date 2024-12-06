#!/usr/bin/env python3
# Advent of Code 2015 Day 2, One-Liner
from collections import deque
from itertools import accumulate, count, takewhile

#print(deque(takewhile(lambda x: x[0] > -2, accumulate(input(), lambda tot, next: (tot[0] + 1, tot[1] + 1) if next == "(" else (tot[0] - 1, tot[1] + 1), initial=(0, 0))), maxlen=1)[0][1])  # noqa

print(deque(takewhile(lambda x: x[0] > -2, accumulate(zip((1 if c == '(' else -1 for c in input()), count(start=1)), lambda tot, next: (tot[0] + next[0], next[1]))), maxlen=1)[0][1])
