#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Advent of Code 2015, Day 11, Part 1
#   https://adventofcode.com/2015/day/11
import re


def inc(c):
    if ord(c) < ord("z"):
        next_c = chr(ord(c) + 1)
        carry = False
    else:
        next_c = "a"
        carry = True
    return (next_c, carry)


def next_pw(s):
    next_c, carry = inc(s[-1])
    if len(s) <= 1:
        return next_c
    elif carry:
        return next_pw(s[:-1]) + next_c
    else:
        return s[:-1] + next_c


def is_good_pw(s):
    rule1 = False
    rule2 = False
    rule3 = False

    for substring in (s[i - 3 : i] for i in range(3, len(s) + 1)):
        ords = [ord(c) for c in substring]
        if ords[1] - ords[0] == 1 and ords[2] - ords[0] == 2:
            rule1 = True

    rule2 = not any([c in s for c in "iol"])

    rule3_ms = re.findall(r"(.)\1", s)
    rule3 = len(re.findall(r"(.)\1", s)) >= 2 and rule3_ms[0] != rule3_ms[1]

    return rule1 and rule2 and rule3


if __name__ == "__main__":
    pw = input()
    pw = next_pw(pw)
    while not is_good_pw(pw):
        pw = next_pw(pw)

    print(pw)
