#!/usr/bin/env python3
import re

line = input()
print(sum((int(s) for s in re.findall(r"-*\d+", line))))
