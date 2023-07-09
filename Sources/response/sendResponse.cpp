/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/09 15:26:03 by hasabir          ###   ########.fr       */
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
	std::cout << clt.response.responseData.data() << std::endl;
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
	
	std::cout << "\033[93mnbr frames = " << clt.response.nbrFrames << std::endl;; 
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
	std::cout << "Frame -> " << clt.response.sizeFrame  << "\033[00m" << std::endl;
	std::vector<char> buffer(clt.response.sizeFrame);

	file.seekg(clt.response.position);
	file.read(buffer.data(), clt.response.sizeFrame);
	// clt.response.position = file.tellg();
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
	try
	{
		long bitRead =
			send(clt.fd, clt.response.responseData.data(), clt.response.responseData.size(), 0);
		std::cout << "bitRead  = " << bitRead << std::endl;
		std::cout << "-------------------------\n";
		if(bitRead < 0)
			throw std::runtime_error("Send operation failed");
		else
		{
			if (clt.response.header)
			{
				std::cout << "***********\n";
				clt.response.position += bitRead;
			}
			else
				clt.response.header = true;
		}
	std::cout << "position = "<< clt.response.position << std::endl; 
	}
	catch (const std::exception& e) 
	{
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }
	if (clt.response.finishReading)
		clt.response.position = 0;
	return statusCode;
}
