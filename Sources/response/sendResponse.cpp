/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/15 12:46:16 by hasabir          ###   ########.fr       */
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
}

void check(struct client &clt, std::ifstream &file, std::string filePath)
{
	if (!file.is_open())
	{
		// std::cerr << "COULD NOT OPEN FILE\n";//!
		error(clt, 500);
		throw std::exception();
	}
	if (clt.response.nbrFrames < 0)
	{
		// std::cout << PURPLE << "INITTING DATA\n" << END;
		initData(clt, filePath, file);
		file.close();
		throw std::exception();
	}
	if (clt.response.position == clt.response.fileSize)
	{
		// std::cout << PURPLE << "FINISH READING\n" << END;
		clt.response.finishReading = true;
		file.close();
		throw std::exception();
	}
	if (clt.response.position < 0)
	{
		// std::cout << PURPLE << "POSITION < 0\n" << END;
		throw std::exception();
	}
}

void	readFile(int statusCode, struct client &clt, std::string filePath)
{
	std::ifstream file;
	std::string chunkHeader;

	file.open(filePath.c_str(), std::ios::binary);
	try {check(clt, file, filePath);}
	catch (std::exception &e){return ;}

	// std::cout << "\033[93mFrame size = " << clt.response.sizeFrame 
	// 	<< " | position = " <<clt.response.position
	// 	<< " | fileSize = " << clt.response.fileSize << "\033[00m" << std::endl;//!

	if (clt.response.position)
		clt.response.sizeFrame = min(clt.response.sizeFrame,
						clt.response.fileSize - clt.response.position);

	std::vector<char> buffer(clt.response.sizeFrame);
	file.seekg(clt.response.position);
	file.read(buffer.data(), clt.response.sizeFrame);
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
	long bitSent;

	if (clt.response.error || clt.response.autoindex)
		fillErrorResponse(clt, web, statusCode);
	else if (!statusCode || (statusCode >= 300 && !clt.response.body))
		fillRedirectResponse(clt, web, statusCode);
	else
	{
		// std::cout << "++++++++++++++++++++++++++++\n";
		fillResponse(clt, web, statusCode);
		// std::cout << "||||||||||||||||||||||||||||\n";
	}
	if (clt.response.finishReading)
	{
		// std::cout << YELLOW <<  "file len = " << clt.response.len 
		// 		<< "| position = " << clt.response.position << END << std::endl;
		clt.response.position = 0;
		return statusCode;
	}
	try
	{
		std::string str(clt.response.responseData.begin(), clt.response.responseData.end());
		char	line[2];
		int n_byte_read = 0;
	 	n_byte_read = recv(clt.fd, line, 0, MSG_PEEK);
		if (n_byte_read < 0)
		{
			clt.response.error = true;
			return 0;
		}
			
		if ((bitSent = send(clt.fd, str.c_str(), str.size(), 0)) <= 0)
		{
			std::cerr << "there is an error\n";
			throw std::runtime_error("Send operation failed");
		}
		// std::cout << "Error code : " << bitSent << " " << n_byte_readed << std::endl; 
	}
	catch(std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}	
	// std::cout << "bitSent  = " << bitSent << std::endl;//!
	if (clt.response.header && bitSent > 0)
		clt.response.position = static_cast<std::streampos>(bitSent + clt.response.position);
	else
		clt.response.header = true;
	return statusCode;
}
