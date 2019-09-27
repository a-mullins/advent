#!/usr/bin/env python3
from sys import stdin
import re


def is_nice(string):
    rule1 = re.search(r'(..).*\1', string)
    rule2 = re.search(r'(.).\1', string)
    return bool(rule1 and rule2)


print(sum((1 for line in stdin.readlines() if is_nice(line))))
