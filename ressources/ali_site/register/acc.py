#! /usr/bin/python3
from http import cookies
import os
import cgi
import time
import hashlib
import pickle
import sys
import yaml

#tmp file for testing cgi from https://github.com/Kaydooo/Webserv_42/blob/main/cgi-bin/acc.py
class Session:
    def __init__(self, name):
        self.name = name
        self.sid = hashlib.sha1(str(time.time()).encode("utf-8")).hexdigest()
        with open('cgi-bin/sessions/session_' + self.sid, 'wb') as f:
            pickle.dump(self, f)
    def getSid(self):
        return self.sid

""" Stores Users and thier data  """
class UserDataBase:
    def __init__(self):
        self.user_pass = {}
        self.user_firstname = {}
    def addUser(self, username, password, firstname):
        self.user_pass[username] = password
        self.user_firstname[username] = firstname
        with open('cgi-bin/user_database', 'wb') as f:
            pickle.dump(self, f)


def read_html_file(file_name):
    with open(file_name, "r") as file:
        file.readline()
        for line in  file:
            print(line)


def printAccPage(session):
    print("Content-type: text/html\r\n")
    print("<html>")
    print("<head>")
    print("<title>Account Page</title>")
    print("</head>")
    print("<body>")
    print("<h1>Welcome Again", session.name, "!</h1>")
    print("<p>Your Session ID is: ", session.getSid(), "</p>")
    print("</body>")
    print("<a href=\"/index.html\"> Click here to go back to homepage </a>")
    print("</html>")

def printUserMsg(msg):
    print("Content-type: text/html\r\n")
    print("<html>")
    print("<head>")
    print("<title>USER MSG</title>")
    print("</head>")
    print("<body>")
    print("<h1>", msg ,"</h1>")
    print("</body>")
    print("<a href=\"../cgi/acc.py\"> Click here to go back to login page </a>")
    print("</html>")

def print_login():
    print("Content-Type: text/html\r\n")
    read_html_file("login.html")

def authUser(name, password):
    if os.path.exists('cgi/accounts'):
        with open('cgi/accounts', 'rb') as f:
            database = pickle.load(f)
            if name in database.user_pass and database.user_pass[name] == password:
                session = Session(database.user_firstname[name])
                return session
            else:
                return None
    else:
        return None

def handle_login():
    username = form.getvalue('username')
    password = form.getvalue('password')
    if username is None:
        print_login()
    elif password is None:
        session = authUser(form.getvalue('username'), form.getvalue('password'))
        if session is None:
            printUserMsg("Failed To Login, Username or Passowrd is wrong!")
        else:
            print("Correct Crenditales :D",file=sys.stderr)
            cookies.clear()
            cookies["SID"] = session.getSid()
            cookies["SID"]["expires"] = 120 # Session Expires after 2 mins
            print("HTTP/1.1 301 OK")
            print(cookies.output())
            print("location: acc.py")
            print("\r\n")
    else:
        with open('accounts.yml', 'rb') as file:
            database = yaml.safe_load(file)
            if username in database:
                printUserMsg("Username is already Registerd !")
            else:
                database[username] = {"password": password, "Credit card": [form.getvalue('cardnumber'), form.getvalue('cardname'), form.getvalue('expdate'), form.getvalue('cvvnumber')]}
                with open('accounts.yml', 'w') as yaml_file:
                    yaml.dump(database, yaml_file)
                printUserMsg("Account Registerd Successfully!")

form = cgi.FieldStorage()
if 'HTTP_COOKIE' in os.environ:
    cookies = cookies.SimpleCookie()
    cookies.load(os.environ["HTTP_COOKIE"])

    if "SID" in cookies:
        print("Your Session ID is", cookies["SID"].value,file=sys.stderr)
        with open('cgi-bin/sessions/session_'+ cookies["SID"].value, 'rb') as f:
            sess = pickle.load(f)
        printAccPage(sess)
    else:
        handle_login()
else:
    handle_login()


