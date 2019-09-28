#!/usr/bin/env python3
"""
Brute force approach to a version of the Traveling Salesman Problem,
but finding the longest route.
"""
import re
from sys import stdin
from itertools import permutations

pattern = re.compile(r'(\w+) to (\w+) = (\d+)')
cities = set()
distances = {}

for line in stdin:
    matches = pattern.search(line).groups()
    cities.update(matches[:2])
    distances[''.join(matches[:2])] = int(matches[2])
    distances[''.join(matches[1::-1])] = int(matches[2])

longest = 0
for route in permutations(cities):
    route_len = 0
    for i in range(1, len(route)):
        route_len += distances[route[i-1] + route[i]]
    if route_len > longest:
        longest = route_len

print(f'Santa must visit {len(cities)} cities:')
for city in cities:
    print(f'\t{city}')

print(f'Longest route among them is {longest} units.')
