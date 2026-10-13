#!/usr/bin/env python3
import re
from sys import stdin

p = re.compile(r'\(\d+(,\d+)*\)')

max_buttons = 0
for line in stdin:
    button_count = len(p.findall(line))
    print(f'{button_count} {line.strip()}')
    if button_count > max_buttons:
        max_buttons = button_count
print()
print(max_buttons)
