#!/usr/bin/env python
from collections import namedtuple
from itertools import chain


SleepRecord = namedtuple("SleepRecord", "date guard_id minutes_asleep")


records = []
all_guards = set()


# parse records
with open('sorted_input', 'r') as fp:
    current_guard = None
    asleep_since = None
    for line in fp:
        if 'begins shift' in line:
            current_guard = int(line.split()[3][1:])
            all_guards.add(current_guard)
            #print(f'Guard {current_guard} begins their shift.')
        if 'falls asleep' in line:
            asleep_since = int(line.split()[1].split(':')[1][:-1])
            #print(f'Fell asleep at {asleep_since}.')
        if 'wakes up' in line:
            date = line.split()[0][-5:]
            awake_since = int(line.split()[1].split(':')[1][:-1])
            records.append(SleepRecord(date, current_guard,
                                       list(range(asleep_since, awake_since))))
            #print(f'Woke up at {awake_since}.')

# find the guard who sleeps the most
sleep_sums = {}
sleepiest = records[0].guard_id
for rec in records:
    # if rec.guard_id not in sleep_sums:
    #     sleep_sums[rec.guard_id] = 0
    sleep_sums[rec.guard_id] = sleep_sums.setdefault(rec.guard_id, 0) \
                               + len(rec.minutes_asleep)
    if sleep_sums[rec.guard_id] > sleep_sums[sleepiest]:
        sleepiest = rec.guard_id
print(f'Sleepiest guard is {sleepiest}.')

# find the minute when they're most likely to be asleep.
sleepy_times = list(chain.from_iterable(
    [rec.minutes_asleep for rec in records if rec.guard_id == sleepiest]))
most_asleep = (0, 0)
for minute in range(60):
    if sleepy_times.count(minute) > most_asleep[1]:
        most_asleep = (minute, sleepy_times.count(minute))

print(f'Guard {sleepiest} likes to sleep most at {most_asleep[0]}. '
      f'{most_asleep[1]} times!\nPuzzle answer: {sleepiest * most_asleep[0]}')
