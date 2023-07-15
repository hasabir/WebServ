#!/usr/bin/env python

import cgi

html_text = '<!DOCTYPE html>\n<html>\n<body>\n'
html_text += '\t<title>'+'test'+'</title>\n'
html_text += '<h3>Hello world by python cgi</h3>'
html_text += '<form action="welcome.py" method="get">'
form = cgi.FieldStorage()
if 'first_name' in form:
    first_name = form['first_name'].value
    html_text += '<center><p><h1>Welcome: ' + first_name + ' '
if 'last_name' in form:
    last_name = form['last_name'].value
    html_text +=  last_name 

html_text += '</h1></p></center></body>\n</html>\n'

print(html_text)