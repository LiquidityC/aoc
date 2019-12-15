#!/bin/env python

from datetime import datetime
from collections import defaultdict
import sys

with open("input.txt") as fh:
    raw_data = fh.readlines()

# Read data
logs = []
for line in raw_data:
    raw = line.strip().split(" ", 2)
    dateStr = " ".join(raw[:2])
    message = " ".join(raw[2:])
    data = (datetime.strptime(dateStr, "[%Y-%m-%d %H:%M]"), message)
    logs.append(data)

# Sort the data
logs = sorted(logs, key=lambda tup: tup[0])


# Parse guard data
guards = defaultdict(lambda:defaultdict(lambda:0))
guard = None
for log in logs:
    tstamp, message = log
    if message == "falls asleep":
        guards[guard][tstamp.minute] += 1
    elif message == "wakes up":
        pass
    else:
        guard = message.split(" ")[1][1:]

# Find guard with highest sleeping rate and at what minute
best_result = (0, 0, 0)
for guard, minutes in guards.items():
    best_minute = max(minutes, key=minutes.get)
    if minutes[best_minute] > best_result[2]:
        best_result = guard, best_minute, minutes[best_minute]
print(best_result)
print("Part 1:", int(best_result[0]) * int(best_result[1]))
