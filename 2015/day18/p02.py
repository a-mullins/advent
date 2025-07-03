#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Solution for Advent of Code 2015, Day 18, Part 2
#   https://adventofcode.com/2015/day/18#part2
from sys import stdin


class LightGrid:
    def __init__(self, init=None):
        if init:
            self.g = init
        else:
            self.g = [[0 for col in range(100)] for row in range(100)]
        self.size = len(self.g)
        for row in (0, self.size - 1):
            for col in (0, self.size - 1):
                self.g[row][col] = 1

    def __str__(self):
        ret_val = ""
        for row in self.g:
            ret_val += "".join((("#" if col else "." for col in row)))
            ret_val += "\n"
        return ret_val

    def step(self):
        next_g = [[0 for col in range(self.size)] for row in range(self.size)]
        for row in range(self.size):
            for col in range(self.size):
                # row 1 col 5 problems
                row_range = range(
                    row - 1 if row > 0 else 0,
                    row + 2 if row < self.size - 1 else self.size,
                )
                col_range = range(
                    col - 1 if col > 0 else 0,
                    col + 2 if col < self.size - 1 else self.size,
                )
                live_neighbors = 0
                for y in row_range:
                    for x in col_range:
                        if self.g[y][x] and not (y == row and x == col):
                            live_neighbors += 1
                # del just in case
                del y, x

                if self.g[row][col]:  # if alive
                    if live_neighbors in (2, 3):
                        next_g[row][col] = 1
                    else:
                        next_g[row][col] = 0
                else:  # if dead
                    if live_neighbors == 3:
                        next_g[row][col] = 1
                    else:
                        next_g[row][col] = 0
        # END for next_row in range ...
        for row in (0, self.size - 1):
            for col in (0, self.size - 1):
                next_g[row][col] = 1
        self.g = next_g


if __name__ == "__main__":
    init = [[1 if c == "#" else 0 for c in row.strip()] for row in stdin]
    lg = LightGrid(init)

    for _ in range(100):
        lg.step()

    print(sum((col for row in lg.g for col in row)))
