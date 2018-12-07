#!/usr/bin/env python
from collections import namedtuple, Counter


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
            print(f'Guard {current_guard} begins their shift.')
        if 'falls asleep' in line:
            asleep_since = int(line.split()[1].split(':')[1][:-1])
            print(f'Fell asleep at {asleep_since}.')
        if 'wakes up' in line:
            date = line.split()[0][-5:]
            awake_since = int(line.split()[1].split(':')[1][:-1])
            records.append(SleepRecord(date, current_guard,
                                       list(range(asleep_since, awake_since))))
            print(f'Woke up at {awake_since}.')

# for every minute, count which guards are asleep on that minute.
# After, determine which guard is most likely to sleep on that minute (ie, find the mode)
# and compare that to the current most likely to be asleep minute & guard.
minute_count = {}     # has fmt {<minute>: [guard_ids]}
most_asleep = (0, 0, 0)  # (minute, guard_id, count)
for m in range(60):
    data = Counter([rec.guard_id for rec in records
                    if m in rec.minutes_asleep])
    try:
        guard_id, count = data.most_common(1)[0]
    except IndexError:
        # no data for this minute
        print(f'No data for minute {m}.')
    else:
        # IndexError not raised -- data available.
        if count > most_asleep[2]:
            most_asleep = (m, guard_id, count)

# Print solution.
print(f'Guard {most_asleep[1]} is most frequently asleep on the same '
      f'minute. In this case, {most_asleep[2]} times on minute '
      f'{most_asleep[1]}.')
print(f'Puzzle solution: {most_asleep[0] * most_asleep[1]}')
