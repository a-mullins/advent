#!/usr/bin/env python3
from sys import argv


LETTERS = "abcdefghijklmnopqrstuvwxyz"


def reduce_polymer(poly: list):
    changed = True
    while changed:  # do while something has been changed...
        changed = False
        for i in range(len(poly)-1):
            if poly[i].swapcase() == poly[i+1]:
                # print(f"\tremoving {''.join(poly[i:i+2])}")
                del poly[i:i+2]
                changed = True
                break  # GO NO FARTHER! list indicies will be screwed up.
    return poly


if __name__ == '__main__':
    with open(argv[1], 'r') as fp:
        polymer = fp.readline().strip()

    for letter in LETTERS:
        print(f'{letter},',
              len(reduce_polymer(
                  [x for x in polymer
                   if not (x == letter or x == letter.swapcase())]
              )))
