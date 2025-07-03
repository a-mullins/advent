#!/usr/bin/env python3
import re

print(sum((int(s) for s in re.findall(r"-*\d+", input()))))
