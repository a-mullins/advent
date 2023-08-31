#!/usr/bin/env python3

print(sum((1 if c == "(" else -1 for c in input())))
