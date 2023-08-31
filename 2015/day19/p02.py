#!/usr/bin/env python3
# Uses a greedy approach that just tries a single 'branch' of
# replacement options, blindly following whichever replacement creates
# the shortest string.

# Occasionally throws exceptions when it follows a dead end and runs out
# of options.
import re
from sys import stdin


def visit_node(molecule: str, replacements: list) -> list:
    results = set()
    for target, replacement in replacements:
        for m in re.finditer(target, molecule):
            results.add(
                molecule[: m.start()] + replacement + molecule[m.end() :]
            )
    return results


if __name__ == "__main__":
    replacements = []
    for line in stdin:
        if "=>" in line:
            parts = line.split()
            replacements.append((parts[-1], parts[0]))
        elif line != "":
            medicine_molecule = line.strip()

    print("Target:\n" + medicine_molecule + "\n")
    print("Replacements:")
    for target, repla in replacements:
        print(f"{target:3} => {repla}")
    print()

    num_steps = 0
    graph_tips = [medicine_molecule]
    # TODO: a more robust algorithm that can backtrack and try different
    #       branches when it runs out of options.
    while "e" not in graph_tips:
        options = visit_node(graph_tips[0], replacements)
        options = list(options)
        options.sort(key=len)
        try:
            graph_tips.insert(0, options[0])
        except IndexError as e:
            print(f"!! options: {options}\n\n")
            print(f"!! graph_tips: {graph_tips}\n\n")
            raise e
        num_steps += 1
        print(num_steps)
