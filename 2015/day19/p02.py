#!/usr/bin/env python3
"""Naive approach. Uses four GiB, still doesn't complete."""
import re
from sys import stdin


def visit_node(molecules: list, replacements: list) -> list:
    results = set()
    for molecule in molecules:
        for target, replacement in replacements:
            for m in re.finditer(target, molecule):
                results.add(molecule[:m.start()] +
                            replacement +
                            molecule[m.end():])
    return results


if __name__ == '__main__':
    replacements = []
    for line in stdin:
        if '=>' in line:
            parts = line.split()
            replacements.append((parts[0], parts[-1]))
        elif line != '':
            medicine_molecule = line.strip()

    print('Target:\n' + medicine_molecule + '\n')
    print('Replacements:')
    for target, repla in replacements:
        print(f'{target:3} => {repla}')
    print()

    s = set()
    s.add('e')
    num_steps = 0
    while medicine_molecule not in s:
        s = visit_node(s, replacements)
        num_steps += 1
        print(num_steps)
    # print(num_steps)
