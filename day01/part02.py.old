#!/usr/bin/env python
import sys

# itertools.cycle would also work, and be fewer LOC but from the
# docstring it seems like it will read the whole iterable into memory.
# Using a loop should(?) be a little more memory effecient.

fp = open(sys.argv[1], 'r')

# sanity check.
# if file is empty, read loop will never finish and do no useful work.
fp.seek(0, 2)  # end of file
if not fp.tell() > 0:
    print("ERR: empty file")
    exit(1)
fp.seek(0, 0)  # start of file


acc = 0
freqs = set()  # search on sets is MUCH faster than lists
freqs.add(acc)
while True:
    line = fp.readline()
    if line == '':            # if EOF
        fp.seek(0)            # go back to top of file.
        line = fp.readline()

    acc += int(line)
    if acc in freqs:
        break
    else:
        freqs.add(acc)

fp.close()
print(acc)
exit(0)
