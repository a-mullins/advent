#!/usr/bin/env python3
from math import floor
import re


def pair_off(s, offset=0):
    l = []
    for i in range(len(s)-1):
        pair = s[i:i+2]
        if len(pair) > 1:
            l.append(pair)
    return l


def is_nice(s):
    reg = re.compile(r'(.).\1')
    has_pair = any([pair in s.replace(pair, '', 1) for pair in pair_off(s)])
    has_rep = reg.search(s)
    print(f'pair: {has_pair}\thas_rep: {has_rep}')
    return bool(has_pair and has_rep)


with open('day05-in.txt', 'rt') as fp:
    lines = fp.readlines()

for line in lines[:10]:
    print(f'{is_nice(line.strip())}: {line.strip()}')
#print(len(list(filter(is_nice, (x.strip() for x in lines)))))
