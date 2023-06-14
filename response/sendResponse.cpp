/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/14 12:12:16 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


void getResponse(struct client &clt, int statusCode, std::string filePath)
{
	// std::string response;

	clt.response.header = true;
	clt.response.responseData = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	switch (statusCode)
	{

		case 302:
			clt.response.responseData += "location: " + clt.map_request["URI"] + "\r\n\r\n";
			return;
		case 301:
			clt.response.responseData += "Connection: close\r\nServer: webserver-c\r\n ";
			clt.response.responseData +=  "Content-type: text/html\r\n\r\n";
			clt.response.responseData += " <html> <h1>Moved Permanently => need to be handled<h1> </html>\r\n";
			return;
		case 0:
			clt.response.responseData += "Connection: \r\nServer: webserver-c\r\n ";
			clt.response.responseData +=  "Content-type: text/html\r\n\r\n";
			clt.response.responseData += " <html> <h1> CGI or Post or delete or something not yet handled <h1>  </html>\r\n";
			return ;
	}
	clt.response.responseData += "Connection: close\r\nServer: webserver-c\r\n ";
	clt.response.responseData += "Content-Type: " + getContentType(filePath) + "\r\n";
	clt.response.responseData += "Content-Length:" + intToString(clt.response.len) +"\r\n\r\n";
	clt.response.responseData.insert(clt.response.responseData.end(),
		clt.response.responseBody.begin(), clt.response.responseBody.end());
}

void	readFile(std::string &filePath, int statusCode, struct client &clt)
{
	std::ifstream	file;
	int				sizeFrame(1024);

	if (filePath == "Default")
		filePath = "www/error/"
					+ intToString(statusCode) + ".html";
	file.open(filePath.c_str(), std::ios::binary);
	if (!file.is_open() && statusCode != 200 && statusCode != -302)
	{
		filePath = "www/error/"
					+ intToString(statusCode) + ".html";
		file.open(filePath.c_str(), std::ios::binary);
	}
	
	/******* Reading the first time *************/
	if (clt.response.nbrFrames < 0)
	{
		file.seekg(0, std::ios::end);
		clt.response.fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		clt.response.nbrFrames = static_cast<unsigned long>(clt.response.fileSize) / 1024;
		clt.response.len = clt.response.fileSize;
		clt.response.position = file.tellg();
	}
	
	if (clt.response.nbrFrames == 0)
	{
		clt.response.finishReading = true;
		sizeFrame = clt.response.fileSize;
	}
	else
	{
		clt.response.fileSize -= sizeFrame;
		clt.response.nbrFrames--;
	}
	std::vector<char> buffer(sizeFrame);
	file.seekg(clt.response.position);

	std::cout << "\033[92mposition = " << clt.response.position << "\033[00m\n";
	std::cout << "\033[92mframe size = " << sizeFrame << "\033[00m\n";

	file.read(buffer.data(), sizeFrame);
	clt.response.responseBody = buffer;
	clt.response.position = file.tellg();
	file.close();
}

std::string getContentType(std::string filePath)
{
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

void fillResponseHeader(struct client &clt, struct webserv &web, int statusCode)
{
	std::cout << "status code = " << statusCode << std::endl;
	std::vector<std::pair<std::string, std::string> >::iterator iter;
	std::string filePath;

	if (clt.location >= 0 )
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
	if (statusCode == 200 || statusCode == -302)
		filePath = clt.map_request["URI"];

	if (!clt.response.header)
		getResponse(clt, statusCode, filePath);
	if (statusCode != 301 && statusCode != 302)
		readFile(filePath, statusCode, clt);
	if (!clt.response.header)
		getResponse(clt, statusCode, filePath);
	else
		clt.response.responseData = clt.response.responseBody.data();
	std::cout << "/------------------------------------------------------------/\n";
	std::cout << clt.response.responseData << std::endl;
	std::cout << "/------------------------------------------------------------/\n";
	if (send(clt.fd, clt.response.responseData.data(),
		clt.response.responseData.size(), 0) < 0)
		std::cout << "ERROR: send\n";
}

//!read
int sendResponse(struct client &clt, struct webserv &web, int statusCode)
{
	fillResponseHeader(clt, web, statusCode);
	return statusCode;
}

