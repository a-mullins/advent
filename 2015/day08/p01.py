#!/usr/bin/env python3
from sys import stdin

code_chars = 0
mem_chars = 0

for line in stdin:
    line = line.strip()
    code_chars += len(line)
    mem_chars += len(eval(line))

print(code_chars - mem_chars)
