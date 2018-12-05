#!/usr/bin/env python
from dataclasses import dataclass
from itertools import combinations

# This part of the challenge boils down to collision detection between axis
# aligned rectangles in two dimensions. Fairly straight forward. Most of the
# work will be massaging the data into a suitable format.
# Approach:
# 1. Define datastructs to store the claims.
# 2. Process textfile into structs.
# 3. Compare every claim to every _other_ claim, detecting collisions.
# 4. Filter claims & print only those which do not collide/overlap.


@dataclass
class Claim:
    id_: int
    x: int; y: int
    w: int; h: int
    overlap: bool


claims = []


# Claim Example:
#   #1 @ 1,3: 4x4
#   <id> @ <x>,<y>: <w>x<h>
with open('input', 'r') as fp:
    for line in fp:
        parts = line.split()
        id_ = int(parts[0][1:])
        loc = (int(n) for n in parts[2][:-1].split(','))
        size = (int(n) for n in parts[-1].split('x'))
        claims.append(Claim(id_, *loc, *size, False))

# Collision detection.
for claim1, claim2 in combinations(claims, 2):
    if claim1.x < claim2.x + claim2.w and \
       claim1.x + claim1.w > claim2.x and \
       claim1.y < claim2.y + claim2.h and \
       claim1.y + claim1.h > claim2.y:
        claim1.overlap = True
        claim2.overlap = True

# Find & print claims that do not have overlaps:
for cl in filter(lambda cl: not cl.overlap, claims):
    print(cl)
