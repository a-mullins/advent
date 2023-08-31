#!/usr/bin/env python3
import sys


def get_code(target_row, target_col):
    row, col = 1, 1
    max_row, max_col = 1, 1
    cur = 20151125

    while not (target_row == row and target_col == col):
        if row == 1:
            row = max_row + 1
            max_row = row
            col = 1
        else:
            row -= 1
            col += 1
        cur = (cur * 252533) % 33554393

    return cur


if __name__ == "__main__":
    parts = sys.stdin.readline().split()
    row = int(parts[parts.index("row") + 1].strip(",."))
    col = int(parts[parts.index("column") + 1].strip(",."))

    print(get_code(row, col))
