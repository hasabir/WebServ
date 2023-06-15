/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendErrorResponse.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 12:01:11 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/15 19:03:04 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void readeErrorFile(struct client &clt, int statusCode)
{
	std::ifstream file;
	std::stringstream responseBody;
	
	file.open(clt.response.filePath.c_str(), std::ios::binary);
	responseBody << file.rdbuf();
	std::copy( std::istreambuf_iterator<char>(responseBody),
	std::istreambuf_iterator<char>(), std::back_inserter(clt.response.responseBody));
	clt.response.error = true;
}


void	getResponseHeaderError(struct client &clt, int statusCode)
{
	std::string response;

	response = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	response += "Connection: close\r\nServer: webserver-c\r\n ";
	response += "Content-Type: " + getContentType(clt.response.filePath) + "\r\n";
	response += "Content-Length:" + intToString(clt.response.responseBody.size()) +"\r\n\r\n";
		clt.response.responseData.assign(response.begin(), response.begin() + response.size());
	clt.response.responseData.insert(clt.response.responseData.end(),
						clt.response.responseBody.begin(), clt.response.responseBody.end());
}

void	fillErrorResponse(struct client &clt, struct webserv &web, int statusCode)
{
	clt.response.filePath = getFilePath(clt, web, statusCode);
	readeErrorFile(clt, statusCode);
	getResponseHeaderError(clt, statusCode);
}