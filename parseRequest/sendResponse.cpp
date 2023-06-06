/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/06 20:49:20 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

std::string readFileContent(std::string &filePath, int statusCode)
{
	std::stringstream responseBody;
	std::fstream file;

	if (filePath == "Default")
		filePath = "/Users/hasabir/Desktop/WebServ/www/error/"
					+ intToString(statusCode) + ".html";
	file.open(filePath.c_str(), std::ios::in);
	if (!file.is_open())
	{
		filePath = "/Users/hasabir/Desktop/WebServ/www/error/"
					+ intToString(statusCode) + ".html";
		file.open(filePath.c_str(), std::ios::in);
	}
	responseBody << file;
	return responseBody.str();
}

std::string getContentType(std::string filePath)
{
	std::string type;
	std::map<std::string, std::string> contentTypes;
	int index;
	
	fillMapContentTypes(contentTypes);
	index = filePath.rfind('.');
	type = filePath.substr(index, filePath.size());
	return "plain";
	
}

void getResponse(struct client &clt, int statusCode,
			std::string &response, std::string responseContent, std::string filePath)
{
	switch (statusCode)
	{
		case 400:
			response = "HTTP/1.1 400 Bad Request\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			// response += "Content-Type: " + getContentType(filePath);
			response += "Content-Length:" + intToString(responseContent.size()) +"\r\n\r\n";
			// response += responseContent;
			response += "400 Bad Request";
			break;
		case 501:
			response = "HTTP/1.1 501 Bad Request\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 23\r\n\r\n";
			// response += responseContent;
			
			response += "501 Not Implemented";
			break;
		case 414:
			response = "HTTP/1.1 414 Request-URI Too Long\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 25\r\n\r\n";
			// response += responseContent;
			response += "414 Request-URI Too Long";
			break;
		case 423:
			response = "HTTP/1.1 413 Request Entity Too Large\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 29\r\n\r\n";
			// response += responseContent;
			response += "413 Request Entity Too Large";
			break;
		case 404:
			response = "HTTP/1.1 404 Not Found\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 14\r\n\r\n";
			// response += responseContent;
			response += "404 Not Found";
			break;
		case 405:
			response = "HTTP/1.1 405 Method Not Allowed\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 23\r\n\r\n";
			// response += responseContent;
			response += "405 Method Not Allowed";
			break;
		case 200:
			response = "HTTP/1.0 200 OK\r\nn";
			response += "Connection: \r\nServer: webserver-c\r\n ";
			response +=  "Content-type: text/html\r\n\r\n";
			// response += responseContent;
			response += " <html> Daba machimochkil  </html>\r\n";
			break;
	}
}

void fillResponseHeader(struct client &clt, struct webserv &web, int statusCode)
{
	std::cout << "status code = " << statusCode << std::endl;
	std::vector<std::pair<std::string, std::string> >::iterator iter;
	std::string response;
	std::string filePath;
	

	if (clt.location >= 0)
	{
		for (iter = web.config[clt.config].location[clt.location].error_page.begin();
		iter != web.config[clt.config].location[clt.location].error_page.end()
		&& iter->first != intToString(statusCode); iter++);
		if (iter != web.config[clt.config].location[clt.location].error_page.end())
			filePath = iter->second;
	}
	if (clt.location < 0
		|| iter == web.config[clt.config].location[clt.location].error_page.end())
	{
		for (iter = web.config[clt.config].error_page.begin();
			iter != web.config[clt.config].error_page.end()
			&& iter->first != intToString(statusCode); iter++);
		if (iter != web.config[clt.config].error_page.end())
			filePath = iter->second;
	}
	if (iter == web.config[clt.config].error_page.end())
		filePath = "default";
	getResponse(clt, statusCode, response, readFileContent(filePath, statusCode), filePath);
	if (statusCode != 200)
		readFileContent(filePath, statusCode);
	getContentType(filePath);
	send(clt.fd, response.c_str(), strlen(response.c_str()), 0);
}


int sendResponse(struct client &clt, struct webserv &web, int statusCode)
{
	fillResponseHeader(clt, web, statusCode);
	return statusCode;
}
