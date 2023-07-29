/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 06:56:52 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/27 15:46:25 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

int handleRedirection(struct client &clt, struct webserv &web)
{
	// char nextChar = clt.map_request["URI"]
	// 				[clt.map_request["URI"].find(web.config[clt.config].location[clt.location].pattern)
	// 				+ web.config[clt.config].location[clt.location].pattern.size()];
	
	// if (nextChar != '/' && nextChar != '\0')
	// 	return error(clt, 404);
	
	clt.map_request["URI"].clear();
	clt.map_request["URI"] = web.config[clt.config].location[clt.location].redirect;

	char whitSpace[] = {'\t', ' ', '\v'};
	size_t whitSpaceChar;
	whitSpaceChar = clt.map_request["URI"].find_first_of(whitSpace, 0, sizeof(whitSpace));
	if (whitSpaceChar != std::string::npos)
	{
		std::stringstream redirect(clt.map_request["URI"]);
		std::string statusCode;
		
		redirect >> statusCode;
		clt.response.statusCode = stringToInt(statusCode);
		if (clt.response.statusCode < 100 || clt.response.statusCode > 599)
			return error(clt, 400);//!The request is malformed
		for (; whitSpaceChar < clt.map_request["URI"].size()
			&& std::isspace(clt.map_request["URI"][whitSpaceChar]); whitSpaceChar++);
		clt.map_request["URI"] = clt.map_request["URI"].substr(whitSpaceChar,
			clt.map_request["URI"].size());
	}
	return 0;
}


int parsLocation(struct client &clt, struct webserv &web)
{
	if ((clt.location = search(clt, web)) < 0)
	{
		if (clt.map_request["URI"] != "/" || web.config[clt.config].root.empty())
			return error(clt, 404);
		clt.map_request["URI"] = web.config[clt.config].root;
		return 0;
	}
	if (!web.config[clt.config].location[clt.location].redirect.empty())
	{
		int status;
		if ((status = handleRedirection(clt, web)))
			return status;
	}
	else if (!web.config[clt.config].location[clt.location].root.empty())
		clt.map_request["URI"] = replaceLocation(clt.map_request["URI"],
										 web.config[clt.config].location[clt.location].pattern, 
										 web.config[clt.config].location[clt.location].root);
	else if (!web.config[clt.config].root.empty())
		clt.map_request["URI"] = replaceLocation(clt.map_request["URI"],
										 web.config[clt.config].location[clt.location].pattern,
										 web.config[clt.config].root);
	else
		return error(clt, 404);
	return 0;
}

int getHostPort(struct client &clt, struct webserv &web)
{
	std::vector<std::string>::iterator	port;
	// struct addrinfo *addrinfo;


	size_t i = 0;
	for (; i < web.config.size(); i++) //! handel host
	{
		if ((clt.map_request["Host"].substr(0,clt.map_request["Host"].find(":"))
			== web.config[i].host)
			|| host(clt.map_request["Host"].substr(0,clt.map_request["Host"].find(":")))
			== web.config[i].host)
		{
			port = std::find(web.config[i].listen.begin(), web.config[i].listen.end(),
				clt.map_request["Host"].substr(clt.map_request["Host"].find(":") + 1));
			if (port != web.config[i].listen.end())
				break;
		}
	}
	if (*port != clt.map_request["Host"].substr(clt.map_request["Host"].find(":") + 1))
	{
		clt.port = web.config[0].listen[0];
		return 0;
	}
	clt.port = *port;
	return i;
}

int isRequestWellFormed(struct client &clt, struct webserv &web)
{
	clt.config = getHostPort(clt, web);
	// std::cout << "---------------------\n";
	if ( (clt.map_request["URI"].find_first_not_of(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%")
		!= std::string::npos)
		|| (clt.map_request["Method"] == "POST" 
		&& clt.map_request.find("Transfer-Encoding") == clt.map_request.end()
		&& clt.map_request.find("Content-Length") == clt.map_request.end()))
		return error(clt, 400);
	if (clt.map_request.find("Transfer-Encoding") != clt.map_request.end()
		&& clt.map_request["Transfer-Encoding"] != "chunked")
	{
		return error(clt, 501);
	}
		
	if (clt.map_request["URI"].size() > 2048)
		return error(clt, 414);

	if (clt.map_request.find("Content-Length") != clt.map_request.end()
		&& clt.map_request["Method"] == "POST"
	&& stringToInt(clt.map_request["Content-Length"])
		> stringToInt(web.config[clt.config].max_body_size))
	{
		// std::cout << "here\n";
		return error(clt, 413);
	}
	return 0;
}

int parseRequestData(struct client &clt, struct webserv &web)
{
	isRequestWellFormed(clt, web);
	if (clt.response.error)
		return 0;

	parsLocation(clt, web);
	if (clt.response.error)
		return 0;

	std::vector<std::string>::iterator iter;
	if (clt.config != -1 && clt.location != -1)
	{
		iter = std::find(web.config[clt.config].location[clt.location].allow.begin(),
		web.config[clt.config].location[clt.location].allow.end(), clt.map_request["Method"]);
		if (iter == web.config[clt.config].location[clt.location].allow.end())
			return error(clt, 405);
		return 0;
	}
	return 0;
}

int parseRequest(struct webserv &web, struct client &clt)
{
	clt.config = -1;
	clt.location = -1;

	fillRequestData(clt);

	parseRequestData(clt, web);
	return 0;
}