import yaml

def printUserMsg(msg):
    print("Content-type: text/html\r\n")
    print("<html>")
    print("<head>")
    print("<title>USER MSG</title>")
    print("</head>")
    print("<body>")
    print("<h1>", msg ,"</h1>")
    print("</body>")
    print("<a href=\"login.html\"> Click here to go back to login page </a>")
    print("</html>")


def read_html_file(file_name):
    with open(file_name, "r") as file:
        file.readline()
        for line in file:
            print(line)


def load_accounts_file(file):
    return yaml.safe_load(file)