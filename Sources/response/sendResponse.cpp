/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/30 14:52:23 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"


void getResponseHeader(struct client &clt, int statusCode, std::string filePath)
{
	std::string response;

	clt.response.header = true;
	response = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	response += "Server: webserver-c\r\n";
	response += "Connection: keep-alive\r\n";
	response += "Content-Type: " + getContentType(filePath) + "\r\n";
	response += "Content-Length:" + intToString(clt.response.len) +"\r\n\r\n";
	clt.response.responseData.assign(response.begin(), response.end());
	clt.response.responseData.insert(clt.response.responseData.end(),
						clt.response.responseBody.begin(), clt.response.responseBody.end());
}

void	readFile(int statusCode, struct client &clt, std::string filePath)
{
	std::ifstream file;
	std::string chunkHeader;

	file.open(filePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "COULD NOT OPEN FILE\n";
		error(clt, 500);
		return ;
	}
	// std::cout << "\033[91m Frame -> " << clt.response.nbrFrames << "\033[00m" << std::endl; 
	if (clt.response.nbrFrames < 0)
		initData(clt, filePath, file);
	if (clt.response.nbrFrames == 0)
	{
		clt.response.finishReading = true;
		clt.response.sizeFrame = clt.response.fileSize;
	}
	else
	{
		clt.response.fileSize -= clt.response.sizeFrame;
		clt.response.nbrFrames--;
	}
	std::vector<char> buffer(clt.response.sizeFrame);

	file.seekg(clt.response.position);
	file.read(buffer.data(), clt.response.sizeFrame);
	clt.response.position = file.tellg();
	
	clt.response.responseBody.clear();
	clt.response.responseBody.insert(clt.response.responseBody.end(),
				buffer.begin(), buffer.end());
	file.close();
}

void fillResponse(struct client &clt, struct webserv &web, int statusCode)
{
	if (!clt.response.header)
		clt.response.filePath = clt.map_request["URI"];
	
	readFile(statusCode, clt, clt.response.filePath);	
	if (!clt.response.header)
		getResponseHeader(clt, statusCode, clt.response.filePath);
	else
	{
		clt.response.responseData.clear();
		clt.response.responseData.assign(
				clt.response.responseBody.begin(), clt.response.responseBody.end());
	}
}



int sendResponse(struct client &clt, struct webserv &web, int statusCode)
{
	if (clt.response.error || clt.response.autoindex)
		fillErrorResponse(clt, web, statusCode);
	else if (!statusCode || (statusCode >= 300 && !clt.response.body))
		fillRedirectResponse(clt, web, statusCode);
	else
		fillResponse(clt, web, statusCode);
	if (clt.response.finishReading)
		clt.response.position = 0;

	// std::cout << "uri = " << clt.map_request["URI"] << std::endl;
	if (send(clt.fd, clt.response.responseData.data(), clt.response.responseData.size(), 0) < 0)
	{
		error(clt, 500);
		std::cerr << "ERROR : SEND\n";
	}
	return statusCode;
}
