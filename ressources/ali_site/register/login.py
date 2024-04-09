#! /usr/bin/python3
import utils
import cgi
from http.cookies import SimpleCookie

def print_login():
    print("Content-Type: text/html\r\n")
    utils.read_html_file("login.html")


def login():
    auth(form.getvalue('username'), form.getvalue('password'))


def auth(username, password):
    with open('accounts.yml', 'rb') as file:
        database = utils.load_accounts_file(file)
        if username in database and database[username]["password"]== password:
            utils.printUserMsg("Logged in")
            cookies.clear()
            cookies["SID"] = 4168
            cookies["SID"]["expires"] = 120 # Session Expires after 2 mins
            print(cookies.output())
            print("\r\n")
        else:
            utils.printUserMsg("Error there is no account with this username and password")


form = cgi.FieldStorage()
cookies = SimpleCookie()
print_login() if form.getvalue('username') is None else login()