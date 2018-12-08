#!/usr/bin/env python
from collections import namedtuple, Counter
from sys import argv


LABELS = 'ABCDEFGHIJKLMNOPQRSTUZWXYZabcdefghijklmnopqrstuzwxyz0123456789'
Coord = namedtuple('Coord', 'label x y')
Point = namedtuple('Point', 'x y')


def distance(c1, c2):
    return abs(c1.x - c2.x) + abs(c1.y - c2.y)


max_x = 0
max_y = 0
i = 0
coords = []
points = []

with open(argv[1], 'r') as fp:
    for line in fp:
        c = Coord(LABELS[i], *(int(x) for x in line.strip().split(',')))
        i += 1
        if c.x > max_x: max_x = c.x
        if c.y > max_y: max_y = c.y
        coords.append(c)

for p in (Point(x, y)
          for x in range(max_x+1)
          for y in range(max_y+1)):
    # for every point, determine the distance to every coordinate.
    distances = [(c.label, distance(p, c)) for c in coords]
    # deteremine the closest coord(s)
    min_dist = min((x for _, x in distances))
    # add the closest coords to the list of points
    points.append((p, [lbl for lbl, dist in distances if dist == min_dist]))

# prune those areas which go off to infinity, ie, those that touch the
# edges of the grid.
inf_areas = set((lbl[0] for p, lbl in points
                 if p.x == 0 or p.x == max_x
                 or p.y == 0 or p.y == max_y))
c = Counter((lbl[0] for p, lbl in points
             if (len(lbl) == 1) and (lbl[0] not in inf_areas)))

print(f'Area {c.most_common(1)[0][0]} is the largest '
      f'with a size of {c.most_common(1)[0][1]}.')
