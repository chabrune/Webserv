#!/usr/bin/env python3

import sys

data = sys.stdin.readline().strip()  # Read a single line and remove newline character

with open("fromPostPython.txt", "w+") as my_file:
    my_file.write(data)