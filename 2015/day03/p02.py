#!/usr/bin/env python3
from sys import stdin

locs = {"santa": [0, 0], "robo": [0, 0]}
houses = {}
houses[(0, 0)] = 1

cur_pilot = "robo"
while True:
    if cur_pilot == "robo":
        cur_pilot = "santa"
    else:
        cur_pilot = "robo"

    c = stdin.read(1)
    if c == ">":
        locs[cur_pilot][0] += 1
    elif c == "<":
        locs[cur_pilot][0] -= 1
    elif c == "^":
        locs[cur_pilot][1] += 1
    elif c == "v":
        locs[cur_pilot][1] -= 1
    else:
        break

    x, y = locs[cur_pilot]
    houses[(x, y)] = houses.get((x, y), 0) + 1

print(f"presents delivered to {len(houses)} houses.")
