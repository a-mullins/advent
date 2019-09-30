#!/usr/bin/env python3
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

    def __str__(self):
        return f"{self.name}\n"\
               f"\t{self.speed} km/s for {self.fly_t} sec, "\
               f"then resting for {self.rest_t} sec.\n"\
               f"\t{'FLYING' if self.is_flying else 'NOT FLYING'}\n"\
               f"\tCovered {self.dist} km so far."

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


if __name__ == '__main__':
    deer = []
    for line in stdin:
        parts = line.split()
        deer.append(Reindeer(parts[0], int(parts[3]),
                             int(parts[6]), int(parts[13])))

    for _ in range(2503):
        for d in deer:
            d.update()

    print(max((d.dist for d in deer)))
