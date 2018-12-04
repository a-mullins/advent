#!/usr/bin/env python
import sys
from functools import reduce

with open(sys.argv[1], 'r') as fp:
    print(reduce(lambda x, y: x + int(y), fp, 0))
