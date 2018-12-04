#!/usr/bin/env python

twos = 0
threes = 0
with open('input', 'r') as fp:
    for line in fp.readlines():
        repeats = list(map(lambda x: line.count(x), set(line)))
        if 2 in repeats: twos += 1
        if 3 in repeats: threes += 1
        # print(f'line: {line.strip()}\n'
        #       f'\trepeats: {repeats}\n'
        #       f'\ttwos, threes: {twos, threes}')

print(f'{twos * threes}')
