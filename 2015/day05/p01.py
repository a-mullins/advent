#!/usr/bin/env python3
from sys import stdin


def is_nice(s):
    VOWELS = 'aeiou'
    vowel_cnt = 0
    run_cnt = 0
    naughty_substring = False

    prev = s[0]
    if prev in VOWELS:
        vowel_cnt += 1
    for cur in s[1:]:
        if prev + cur in ['ab', 'cd', 'pq', 'xy']:
            naughty_substring = True
            break
        if cur in VOWELS:
            vowel_cnt += 1
        if cur == prev:
            run_cnt += 1
        prev = cur

    return run_cnt >= 1 and vowel_cnt >= 3 and not naughty_substring


print(len([line for line in stdin.readlines() if is_nice(line)]))
