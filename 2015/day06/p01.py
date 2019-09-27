#!/usr/bin/env python3
import re
from sys import stdin


class LightGrid:
    def __init__(self):
        self.g = [[0 for col in range(1000)] for row in range(1000)]

    # all of these functions will behave badly
    # if p2.col < p1.col or p2.row < p2.col
    def turn_on(self, p1, p2):
        for row in range(p1[0], p2[0]+1):
            for col in range(p1[1], p2[1]+1):
                self.g[row][col] = 1

    def turn_off(self, p1, p2):
        for row in range(p1[0], p2[0]+1):
            for col in range(p1[1], p2[1]+1):
                self.g[row][col] = 0

    def toggle(self, p1, p2):
        for row in range(p1[0], p2[0]+1):
            for col in range(p1[1], p2[1]+1):
                self.g[row][col] ^= 1

    def num_lights_on(self):
        return sum((col for row in self.g for col in row))


if __name__ == '__main__':
    lg = LightGrid()

    for line in stdin.readlines():
        # Avoid issues on empty lines...
        if not line:
            break

        # Extract coord pairs.
        m = re.search(r'(\d+,\d+)\D*(\d+,\d+)', line)
        p1 = tuple(int(x) for x in m.group(1).split(','))
        p2 = tuple(int(x) for x in m.group(2).split(','))
        assert p1 < p2

        # Execute
        if 'turn on' in line:
            lg.turn_on(p1, p2)
        elif 'turn off' in line:
            lg.turn_off(p1, p2)
        elif 'toggle' in line:
            lg.toggle(p1, p2)

    print(lg.num_lights_on())
