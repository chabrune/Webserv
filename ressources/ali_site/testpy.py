import os
import sys


print("<html>")
print("<head>")
print("<title>Hello World - First CGI Program</title>")
print("</head>")
print("<body>")
print("<h2>Hello World! This is my first CGI program</h2>")
print("</body>")
print("</html>")
with open("ooooo", "r") as file:
    print(file.readlines())
for key, value in os.environ.items():
    print(f"{key}={value}")