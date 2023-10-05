#!/usr/bin/env python3
# I feel like there is an analytical solution to this particular
# problem, but I will just simulate it.
from sys import stdin


class Reindeer:
    def __init__(self, name, speed, fly_t, rest_t):
        self.name = name
        self.speed = speed
        self.fly_t = fly_t
        self.rest_t = rest_t

        self.is_flying = True
        self.dist = 0
        self.timer = 0

    def __repr__(self):
        return f"Reindeer({self.name}, ...)"

    def update(self):  # call once every second
        self.timer += 1
        if self.is_flying:
            self.dist += self.speed
            if self.timer >= self.fly_t:
                self.timer = 0
                self.is_flying = False
        else:
            if self.timer >= self.rest_t:
                self.timer = 0
                self.is_flying = True


if __name__ == "__main__":
    scoreboard = {}
    for line in stdin:
        parts = line.split()
        deer = Reindeer(parts[0], int(parts[3]), int(parts[6]), int(parts[13]))
        scoreboard[deer] = 0

    for _ in range(2503):
        for deer in scoreboard.keys():
            deer.update()
        lead_dist = max((d.dist for d in scoreboard.keys()))
        for deer in scoreboard.keys():
            if deer.dist >= lead_dist:
                scoreboard[deer] += 1

    print(max(scoreboard.values()))
