/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/02 21:24:28 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

std::string readFileContent(std::string filePath)
{
	std::string responseBody;
	std::fstream file;
	file.open(filePath.c_str(), std::ios::in);
	if (!file.is_open())
		return "Ther is a problem in opening html file";
	getline(file, responseBody, '\0');
	return responseBody;
}


void getDefaultFile(struct client &clt, int statusCode, std::string &response)
{
	switch (statusCode)
	{
		case 400:
		{
			response = "HTTP/1.1 400 Bad Request\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 16\r\n\r\n";
			response += "400 Bad Request";
		}
		case 501:
		{
			response = "HTTP/1.1 501 Bad Request\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 16\r\n\r\n";
			response += "501 Bad Request";
		}
		case 414:
		{
			response = "HTTP/1.1 414 Request-URI Too Long\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 25\r\n\r\n";
			response += "414 Request-URI Too Long";
		}
		case 423:
		{
			response = "HTTP/1.1 413 Request Entity Too Large\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 29\r\n\r\n";
			response += "413 Request Entity Too Large";
		}
		case 404:
		{
			response = "HTTP/1.1 404 Not Found\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 14\r\n\r\n";
			response += "404 Not Found";
		}
		case 405:
		{
			response = "HTTP/1.1 405 Method Not Allowed\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 23\r\n\r\n";
			response += "405 Method Not Allowed";
		}
		
	
	default:
		break;
	}
}

void fillResponseHeader(struct client &clt, struct webserv &web, int statusCode)
{
		// clt.file->seekg(0, std::ios::end);
		// std::streampos fileSize = clt.file->tellg();
		// clt.file->seekg(0, std::ios::beg);
	std::string response;
	std::fstream file;
	// response = "Connection: close\r\nServer: webserver-c\r\n "; 
	if (clt.location < 0 || web.config[clt.config].error_page.empty())
	{
		// response += " Content-type: text/html\r\n";
		getDefaultFile(clt, statusCode, response);
		send(clt.fd, response.c_str(), strlen(response.c_str()), 0);
	}
}


int sendResponse(struct client &clt, struct webserv &web, int statusCode)
{
	fillResponseHeader(clt, web, statusCode);
	return statusCode;
}

int send_404(struct client &clt) {

	std::cout <<"content file = " << readFileContent("/Users/hasabir/Desktop/WebServ/www/error/404.html") << std::endl;
	clt.response_is_ready = true;
	const char *c404 = "HTTP/1.1 404 Not Found\r\n"
		"Connection: close\r\n"
		"Content-Length: 9\r\n\r\nNot Found";
	send(clt.fd , c404, strlen(c404), 0);
	// closeConnection(web, i);
	return 404;
	// drop_client(client);
}