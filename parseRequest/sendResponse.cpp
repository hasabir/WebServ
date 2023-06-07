/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/07 14:06:55 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"



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
	if (statusCode != 200 && statusCode != 301)
		readFileContent(filePath, statusCode);
	getResponse(clt, statusCode, response, readFileContent(filePath, statusCode), filePath);
	// std::cout << "type = " << getContentType(filePath) << std::endl;
	send(clt.fd, response.c_str(), strlen(response.c_str()), 0);
}


int sendResponse(struct client &clt, struct webserv &web, int statusCode)
{
	fillResponseHeader(clt, web, statusCode);
	return statusCode;
}
