#!/usr/bin/env python3
import cgi
import random

print("Content-type: text/html\n")

form = cgi.FieldStorage()
guess = form.getvalue("guess")
number = random.randint(1, 10)  # Choix d'un nombre entre 1 et 10

html_content = f"""
<html>
<head>
    <title>Guess the Number Game</title>
</head>
<body>
    <h1>Welcome to Guess the Number!</h1>
    <p>I'm thinking of a number between 1 and 10.</p>
"""

if guess:
    guess = int(guess)
    if guess == number:
        html_content += "<p>Congratulations! You guessed it right!</p>"
    else:
        html_content += f"<p>Sorry, that's not it. I was thinking of {number}.</p>"

html_content += """
    <form action="hello.py" method="post">
        <label for="guess">Your guess:</label>
        <input type="number" id="guess" name="guess">
        <input type="submit" value="Submit">
    </form>
</body>
</html>
"""

print(html_content)
