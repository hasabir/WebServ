/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/09 17:49:59 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"


void getResponseHeader(struct client &clt, int statusCode, std::string filePath)
{
	std::string response;

	response = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	response += "Server: webserver-c\r\n";
	response += "Connection: keep-alive\r\n";
	response += "Content-Type: " + getContentType(filePath) + "\r\n";
	response += "Content-Length: " + intToString(clt.response.len) +"\r\n\r\n";
	clt.response.responseData.assign(response.begin(), response.end());
	// std::cout << clt.response.responseData.data() << std::endl;
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
	
	if (clt.response.nbrFrames < 0)
	{
		initData(clt, filePath, file);
		file.close();
		return;
	}
	
	std::cout << "\033[93mlen file = " << clt.response.len 
		<< " | position = " <<clt.response.position
		<< " | fileSize = " << clt.response.fileSize << "\033[00m" << std::endl;
	std::cout << "\033[92mlen - position = " << clt.response.len - clt.response.position << "\033[00m\n";
	
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
	// std::cout << "\033[93mnbr frames = " << clt.response.nbrFrames 
	// << " | position = " <<clt.response.position << "\033[00m" << std::endl;
	file.read(buffer.data(), clt.response.sizeFrame);
	clt.response.position = file.tellg();
	clt.response.responseData.clear();
	clt.response.responseData.assign(buffer.begin(), buffer.end());
	file.close();
}

void fillResponse(struct client &clt, struct webserv &web, int statusCode)
{
	if (!clt.response.header)
		clt.response.filePath = clt.map_request["URI"];
	readFile(statusCode, clt, clt.response.filePath);
	if (!clt.response.header)
		getResponseHeader(clt, statusCode, clt.response.filePath);
}



int sendResponse(struct client &clt, struct webserv &web, int statusCode)
{
	if (clt.response.error || clt.response.autoindex)
		fillErrorResponse(clt, web, statusCode);
	else if (!statusCode || (statusCode >= 300 && !clt.response.body))
		fillRedirectResponse(clt, web, statusCode);
	else
		fillResponse(clt, web, statusCode);
	// std::cout << clt.response.responseData.data() << std::endl;//!
	long bitSent =
		send(clt.fd, clt.response.responseData.data(), clt.response.responseData.size(), 0);
	std::cout << "bitRead  = " << bitSent << std::endl;
	std::cout << "-------------------------\n";
	if(bitSent < 0)
	{
		std::cerr << "error send\n";
		// error(clt, 500);
		// throw std::runtime_error("Send operation failed");
	}
	else
	{
		if (clt.response.header)
			clt.response.position += bitSent;
		else
			clt.response.header = true;
	}
	// std::cout << "position = "<< clt.response.position << std::endl; 
	if (clt.response.finishReading)
	{
		std::cout << "file len = " << clt.response.len << " | position = " << clt.response.position << std::endl;
		clt.response.position = 0;
	}
	// std::cout << "*********************************************\n";
	return statusCode;
}
