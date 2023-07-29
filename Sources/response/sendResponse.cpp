/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/27 19:31:25 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"


void getResponseHeader(struct client &clt, int statusCode, std::string filePath)
{
	std::string response;

	response = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	response += "Server: webserver-c\r\n";
	response += "Connection: keep-alive\r\n";
	response += "Content-Type: " + getContentType(filePath) + CRLF;
	response += "Content-Length: " + intToString(clt.response.len) + CRLF;
	if (clt.response.cgi)
		response += clt.cgi.header;
	response += CRLF;
	clt.response.responseData.assign(response.begin(), response.end());
}

void check(struct client &clt, std::ifstream &file, std::string filePath)
{
	if (!file.is_open())
	{
		error(clt, 500);//TODO
		throw std::exception();
	}
	if (clt.response.nbrFrames < 0)
	{
		initData(clt, filePath, file);
		file.close();
		throw std::exception();
	}
	if (clt.response.position == clt.response.fileSize)
	{
		clt.response.finishReading = true;
		file.close();
		throw std::exception();
	}
	if (clt.response.position < 0)
		throw std::exception();
}

void	readFile(int statusCode, struct client &clt, std::string filePath)
{
	std::ifstream file;
	std::string chunkHeader;

	(void)statusCode;
	file.open(filePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		error(clt, 500);
		return;
	}
	
	try {check(clt, file, filePath);}
	catch (std::exception &e){return ;}


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
	(void)web;
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
	else if (statusCode == 201 || (statusCode >= 300 && !clt.response.body))
		fillRedirectResponse(clt, web, statusCode);
	else
	{
		fillResponse(clt, web, statusCode);
		if (clt.response.error || clt.response.autoindex)
			fillErrorResponse(clt, web, statusCode);
		
	}
	if (clt.response.finishReading && !clt.response.redirection)
	{
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
			clt.response.finishReading = true;
			return 0;
		}
		if ((bitSent = send(clt.fd, str.c_str(), str.size(), 0)) <= 0)
			clt.response.finishReading = 1;
			// throw std::runtime_error("Send operation failed");
	}
	catch(std::exception &e){}
	if (clt.response.header && bitSent > 0)
		clt.response.position = static_cast<std::streampos>(bitSent + clt.response.position);
	else
		clt.response.header = true;
	return statusCode;
}