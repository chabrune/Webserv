#! /usr/bin/python3

import os
from http import cookies
import cgi, cgitb

cookie = cookies.SimpleCookie()
form = cgi.FieldStorage()

def set_cookie(key, value):
    cookie[key] = value
    print("HTTP/1.1 204 OK")
    print(cookie.output())
    print("\r\n")
    print("Cookie added")
    return_button()


def get_cookie(key):
    if 'HTTP_COOKIE' in os.environ:
        cookie.load(os.environ["HTTP_COOKIE"])
    print("Content-Type: text/html\n")
    if key in cookie:
        print("The Value of Cookie", key, "is", cookie[key].value)
    else:
        print("Cookie was not found !")
    return_button()


def return_button():
    print("<a href=\"cookie.html\">")
    print("<button>Click here to return</button>")
    print("</a>")


keyVal = form.getvalue('key')
value = form.getvalue('value')
if value is None:
    get_cookie(keyVal)
else:
    set_cookie(keyVal, value)