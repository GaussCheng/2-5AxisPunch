#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import xml.etree.ElementTree as ET
import csv

if len(sys.argv) < 5:
    print("Usage: python3 autots.py in_zh_ts in_targart_ts tsmapcsv out_file")
    exit
print(sys.argv[1])
inZhTSContexts = ET.parse(sys.argv[1]).getroot().findall("context")

#print(len(inZhTSMessages))

zhToSourcesMap = {}
sourceToZhsMap = {}
for context in inZhTSContexts:
    messages = context.findall("message")
    for message in messages:
        zh = message.find("translation").text
        source = message.find("source").text
        if zh in zhToSourcesMap:
            if source not in zhToSourcesMap[zh]:
                zhToSourcesMap[zh].append(source)
        else:
            zhToSourcesMap[zh] = [source]
        if source in sourceToZhsMap:
            if zh not in sourceToZhsMap[source]:
                sourceToZhsMap[source].append(zh)
        else:
            sourceToZhsMap[source] = [zh]

reader = csv.reader(open(sys.argv[3], 'r'))
zh = ""
ts = ""
zhToTSsMap = {}
for line in reader:
    if len(line) >= 2:
        zh = line[0]
        ts = line[1]
        if zh in zhToTSsMap:
            if ts not in zhToTSsMap[zh]:
                zhToTSsMap[zh].append(ts)
        else:
            zhToTSsMap[zh] = [ts]

inTargetTs =  ET.parse(sys.argv[2])
inTargetContexts = inTargetTs.getroot().findall("context")

for context in inTargetContexts:
    messages = context.findall("message")
    for message in messages:
        source = message.find("source")
        ts = message.find("translation")
        if source.text in sourceToZhsMap:
            zhs = sourceToZhsMap[source.text]
            if zhs[0] in zhToTSsMap:
                ts.clear()
                ts.text = zhToTSsMap[zhs[0]][0]
            print(source.text, ts.text)

inTargetTs.write(sys.argv[4], "UTF-8", True)
