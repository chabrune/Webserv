import os

print("enter")
with open("ooooo", "r") as file:
    print(file.readlines())
for key, value in os.environ.items():
    print(f"{key}={value}")