#!/usr/bin/env python3
import re

from sys import stdin


g = [[0 for col in range(1000)] for row in range(1000)]


# all of these functions will behave badly
# if p2.col < p1.col or p2.row < p2.col
def turn_on(p1, p2):
    for row in range(p1[0], p2[0]+1):
        for col in range(p1[1], p2[1]+1):
            g[row][col] += 1


def turn_off(p1, p2):
    for row in range(p1[0], p2[0]+1):
        for col in range(p1[1], p2[1]+1):
            if g[row][col] >= 1:
                g[row][col] -= 1


def toggle(p1, p2):
    for row in range(p1[0], p2[0]+1):
        for col in range(p1[1], p2[1]+1):
            g[row][col] += 2


while True:
    line = stdin.readline()
    if not line:
        break

    if 'turn on' in line:
        command = turn_on
    elif 'turn off' in line:
        command = turn_off
    elif 'toggle' in line:
        command = toggle
    else:
        break

    # Extract coord pairs.
    m = re.search(r'(\d+,\d+)\D*(\d+,\d+)', line)
    p1 = tuple(int(x) for x in m.group(1).split(','))
    p2 = tuple(int(x) for x in m.group(2).split(','))
    assert p1 < p2

    # Execute
    command(p1, p2)

print(sum((col for row in g for col in row)))
