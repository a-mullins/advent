#!/usr/bin/env python3
from sys import stdin

orig_chars = 0
encoded_chars = 0
table = str.maketrans({'"': r'\"', '\\': '\\\\'})

for line in stdin:
    line = line.strip()
    orig_chars += len(line)
    encoded_chars += len(line.translate(table)) + 2

print(encoded_chars - orig_chars)
