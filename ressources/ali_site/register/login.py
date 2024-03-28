#! /usr/bin/python3
import utils
import cgi
from http.cookies import SimpleCookie

def print_login():
    print("Content-Type: text/html\r\n")
    utils.read_html_file("login.html")


def loginold():
    session = authUser(form.getvalue('username'), form.getvalue('password'))
    if session is None:
        utils.printUserMsg("Failed To Login, Username or Passowrd is wrong!")
    else:
        print("Correct Crenditales :D",file=sys.stderr)
        cookies.clear()
        cookies["SID"] = session.getSid()
        cookies["SID"]["expires"] = 120 # Session Expires after 2 mins
        print("HTTP/1.1 301 OK")
        print(cookies.output())
        print("location: acc.py")
        print("\r\n")

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
            print("HTTP/1.1 301 OK")
            print(cookies.output())
            print("location: login.py")
            print("\r\n")
        else:
            utils.printUserMsg("Error there is no account with this username and password")


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

form = cgi.FieldStorage()
cookies = SimpleCookie()
print_login() if form.getvalue('username') is None else login()