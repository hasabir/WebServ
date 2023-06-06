/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/06 11:55:29 by hp               ###   ########.fr       */
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


void getDefaultFile(struct client &clt, int statusCode, std::string &response, std::string filePath)
{
	std::cout << "\n at least i am here\n";
	switch (statusCode)
	{
		case 400:
			response = "HTTP/1.1 400 Bad Request\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 16\r\n\r\n";
			response += "400 Bad Request";
			break;
		case 501:
			response = "HTTP/1.1 501 Bad Request\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 23\r\n\r\n";
			response += "501 Not Implemented";
			break;
		case 414:
			response = "HTTP/1.1 414 Request-URI Too Long\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 25\r\n\r\n";
			response += "414 Request-URI Too Long";
			break;
		case 423:
			response = "HTTP/1.1 413 Request Entity Too Large\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 29\r\n\r\n";
			response += "413 Request Entity Too Large";
			break;
		case 404:
			std::cout << "i am suppose to be here\n";
			response = "HTTP/1.1 404 Not Found\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 14\r\n\r\n";
			response += "404 Not Found";
			break;
		case 405:
			std::cout << "i am not suppose to be here\n";
			response = "HTTP/1.1 405 Method Not Allowed\r\n";
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response += "Content-Length: 23\r\n\r\n";
			response += "405 Method Not Allowed";
			break;
		case 200:
			response = "HTTP/1.0 200 OK\r\nn";
			response += "Connection: \r\nServer: webserver-c\r\n ";
			response +=  "Content-type: text/html\r\n\r\n";
			response += " <html> Daba machimochkil  </html>\r\n";
			break;
	}
}
void locationNotExist(std::string &fileName, struct client &clt, struct webserv &web, int statusCode)
{
	
}


void fillResponseHeader(struct client &clt, struct webserv &web, int statusCode)
{
	std::cout << "status code = " << statusCode << std::endl;
	std::vector<std::pair<std::string, std::string> >::iterator iter;
	std::string response;
	std::string filePath;
	
	if (clt.location && (iter = 
		std::find(web.config[clt.config].location[clt.location].error_page.begin(),
		web.config[clt.config].location[clt.location].error_page.end(),
		intToString(statusCode)))!= web.config[clt.config].location[clt.location].error_page.end())
		filePath = iter->second;
	else if ((iter = std::find(web.config[clt.config].error_page.begin(),
		web.config[clt.config].error_page.end(),
		intToString(statusCode)))!= web.config[clt.config].error_page.end())
		filePath = iter->second;
	else
		filePath = "";
	
	getDefaultFile(clt, statusCode, response, "");
	send(clt.fd, response.c_str(), strlen(response.c_str()), 0);
	// }
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