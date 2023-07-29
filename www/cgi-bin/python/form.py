#!/usr/bin/env python

html_text = '<!DOCTYPE html>\n<html>\n<head>\n'
html_text += '\t<title>'+'test'+'</title>\n'
html_text += '</head>\n\n'
html_text += '<body>\n'

html_text += '<h3>Hello world by python cgi</h3>'
html_text += '<form action="welcome.py" method="get">'
html_text +=   'First Name: <input type="text" name="first_name">  <br />'
html_text +=   'Last Name: <input type="text" name="last_name" />'
html_text +=   '<input type="submit" value="Submit" />'
html_text +=  '</form>\n</body>\n</html>\n'


print(html_text)