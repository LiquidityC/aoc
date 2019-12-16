#!/bin/env python

from datetime import datetime
from collections import defaultdict
import sys

with open("input.txt") as fh:
    raw_data = fh.readlines()

# Read data
logs = []
for line in raw_data:
    raw = line.rstrip().split(" ", 2)
    dateStr = " ".join(raw[:2])
    message = " ".join(raw[2:])
    data = (datetime.strptime(dateStr, "[%Y-%m-%d %H:%M]"), message)
    logs.append(data)

# Sort the data
logs.sort()

# Parse guard data
guards = defaultdict(lambda:defaultdict(lambda:0))
guard = None
sleep = -1
for log in logs:
    tstamp, message = log
    if message == "falls asleep":
        sleep = tstamp.minute
    elif message == "wakes up":
        assert sleep >= 0
        for m in range(sleep, tstamp.minute):
            guards[guard][m] += 1
    else:
        guard = int(message.split(" ")[1][1:])

# Find guard with highest sleeping rate and at what minute
best_result = (0, 0, 0, 0)
for guard, minutes in guards.items():
    total_sleep = sum(minutes.values())
    if best_result[1] < total_sleep:
        best_minute = max(minutes, key=minutes.get)
        best_result = (guard, total_sleep, best_minute, minutes[best_minute])
print("Part 1:", best_result[0] * best_result[2])

best_result = (0, 0, 0)
for guard, minutes in guards.items():
    best_minute = max(minutes, key=minutes.get)
    if best_result[2] < minutes[best_minute]:
        best_result = guard, best_minute, minutes[best_minute]
print("Part 2:", best_result[0] * best_result[1])
