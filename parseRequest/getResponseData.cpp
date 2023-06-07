/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getResponseData.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 13:49:11 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/07 14:06:58 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void getResponse(struct client &clt, int statusCode,
			std::string &response, std::string responseContent, std::string filePath)
{
	switch (statusCode)
	{
		case 400:
			response = "HTTP/1.1 400 Bad Request\r\n";
			break;
		case 501:
			response = "HTTP/1.1 501 Bad Request\r\n";
			break;
		case 414:
			response = "HTTP/1.1 414 Request-URI Too Long\r\n";
			break;
		case 423:
			response = "HTTP/1.1 413 Request Entity Too Large\r\n";
			break;
		case 404:
			response = "HTTP/1.1 404 Not Found\r\n";
			break;
		case 405:
			response = "HTTP/1.1 405 Method Not Allowed\r\n";
			break;
		case 301:
			response = "HTTP/1.1 302 Moved Permanently\r\n";
			response += "Connection: \r\nServer: webserver-c\r\n ";
			response +=  "Content-type: text/html\r\n\r\n";
			response += " <html> <h1>Moved Permanently => need to be handled<h1> </html>\r\n";
			return;
		case 200:
			response = "HTTP/1.0 200 OK\r\nn";
			response += "Connection: \r\nServer: webserver-c\r\n ";
			response +=  "Content-type: text/html\r\n\r\n";
			response += " <html> Daba machimochkil  </html>\r\n";
			return ;
	}
	response += "Connection: close\r\nServer: webserver-c\r\n ";
	response += "Content-Type: " + getContentType(filePath) + "\r\n";
	response += "Content-Length:" + intToString(responseContent.size()) +"\r\n\r\n";
	response += responseContent;
}

std::string readFileContent(std::string &filePath, int statusCode)
{
	std::string responseBody;
	std::fstream file;

	if (filePath == "Default")
		filePath = "www/error/"
					+ intToString(statusCode) + ".html";
	file.open(filePath.c_str(), std::ios::in);
	if (!file.is_open())
	{
		filePath = "www/error/"
					+ intToString(statusCode) + ".html";
		file.open(filePath.c_str(), std::ios::in);
	}
	getline(file, responseBody, '\0');
	return responseBody;
}

std::string getContentType(std::string filePath)
{;
	std::map<std::string, std::string> contentTypes;
	std::map<std::string, std::string>::iterator iter;
	std::string type;
	int index;
	
	fillMapContentTypes(contentTypes);
	index = filePath.rfind('.');
	type = filePath.substr(index, filePath.size());
	if ((iter = contentTypes.find(type)) != contentTypes.end())
		return iter->second;
	return "application/octet-stream";
}