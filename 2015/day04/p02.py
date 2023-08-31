#!/usr/bin/env python3
from hashlib import md5

secret = input()

n = 0
while True:
    n += 1
    m = md5()
    m.update(bytes(secret + str(n), "utf8"))
    digest = m.hexdigest()
    if digest[:6] == "000000":
        print(f"{secret} + {n}: {digest}")
        break
