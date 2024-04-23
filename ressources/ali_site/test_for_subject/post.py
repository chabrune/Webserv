#!/usr/bin/env python3

import sys

# Set the content type to HTML
print("Content-Type: text/html\n")

# HTML boilerplate
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>Result</title>")
print("</head>")
print("<body>")

data = sys.stdin.readline().strip()  # Read a single line and remove newline character

print("Data received:", data)

print("<a href=\"testmain.html\">")
print("<button>Click here to return</button>")
print("</a>")

print("</body>")
print("</html>")