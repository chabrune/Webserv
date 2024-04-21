import cgi
import yaml
import utils

form = cgi.FieldStorage()

username = form.getvalue('username')
password = form.getvalue('password')
with open('accounts.yml', 'rb') as file:
    database = utils.load_accounts_file(file)
if username in database:
    utils.printUserMsg("Username is already Registerd !")
else:
    database[username] = {"password": password, "Credit card": [form.getvalue('cardnumber'), form.getvalue('cardname'), form.getvalue('expdate'), form.getvalue('cvvnumber')]}
    with open('accounts.yml', 'w') as yaml_file:
        yaml.dump(database, yaml_file)
    utils.printUserMsg("Account Registerd Successfully!")