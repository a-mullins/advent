#!/usr/bin/env python3
import re
from sys import stdin


def is_nice(s):
    naughties = ("ab", "cd", "pq", "xy")
    rule1 = len(re.findall(r"[aeiou]", s)) >= 3
    rule2 = re.search(r"(.)\1", s)
    rule3 = not any((sub in s for sub in naughties))
    return bool(rule1 and rule2 and rule3)


print(sum((1 for line in stdin.readlines() if is_nice(line))))
