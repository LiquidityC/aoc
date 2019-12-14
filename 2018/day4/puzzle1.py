#!/bin/env python

from datetime import datetime
import sys

readFromFile = len(sys.argv) > 1

# Determine where to fetch data
inf = None
if readFromFile:
    inf = open(sys.argv[1])
else:
    inf = sys.stdin

# Read data
logs = []
for line in inf:
    raw = line.strip().split(" ", 2)
    dateStr = " ".join(raw[:2])
    message = " ".join(raw[2:])
    data = (datetime.strptime(dateStr, "[%Y-%m-%d %H:%M]"), message)
    logs.append(data)

if readFromFile:
    inf.close()

# Sort the data
logs = sorted(logs, key=lambda tup: tup[0])

#for log in logs:
    #print(datetime.strftime(log[0], "[%Y-%m-%d %H:%M]"), log[1])

# Parse guard data
guards = {}
for log in logs:
    
