/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 06:56:52 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/15 21:43:13 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int parsLocation(struct client &clt, struct webserv &web)
{
	if ((clt.location = search(clt, web)) < 0)
	{
		if (clt.map_request["URI"] != "/" || web.config[clt.config].root.empty())
			return 404;
		clt.map_request["URI"] = web.config[clt.config].root;
		return 0;
	}
	if (!web.config[clt.config].location[clt.location].redirect.empty())
	{
		clt.map_request["URI"] = web.config[clt.config].location[clt.location].redirect;
		char whitSpace[] = {'\t', ' ', '\v'};
		if (clt.map_request["URI"].find_first_of(whitSpace, 0, sizeof(whitSpace)) != std::string::npos)
		{
			std::stringstream redirect(clt.map_request["URI"]);
			std::string statusCode;

			redirect >> statusCode;
			redirect >> clt.map_request["URI"];
			clt.response.statusCode = stringToInt(statusCode);
			std::cout << "statusCode = " << clt.response.statusCode << "\n";
			//!301
		}
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
		return 404;
	return 0;
}

int getHostPort(struct client &clt, struct webserv &web)
{
	std::vector<std::string>::iterator	port;
	struct addrinfo *addrinfo;

	int i = 0;
	for (; i < web.config.size(); i++) //! handel host
	{
		if (clt.map_request["Host"].substr(0,clt.map_request["Host"].find(":"))
			== host(web.config[i].host))
		{
			port = std::find(web.config[i].listen.begin(), web.config[i].listen.end(),
				clt.map_request["Host"].substr(clt.map_request["Host"].find(":") + 1));
			if (port != web.config[i].listen.end())
				break;
		}
	}
	if (*port != clt.map_request["Host"].substr(clt.map_request["Host"].find(":") + 1))
		return 0;
	return i;
}

int isRequestWellFormed(struct client &clt, struct webserv &web)
{
	clt.config = getHostPort(clt, web);
	if ( (clt.map_request["URI"].find_first_not_of(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%")
		!= std::string::npos)
		|| (clt.map_request["Method"] == "POST" 
		&& clt.map_request.find("Transfer-Encoding") == clt.map_request.end()
		&& clt.map_request.find("Content-Length") == clt.map_request.end()))
		return 400;
	if (clt.map_request.find("Transfer-Encoding") != clt.map_request.end()
		&& clt.map_request["Transfer-Encoding"] != "chunked")
		return 501;
		
	if (clt.map_request["URI"].size() > 2048)
		return 414;

	if (clt.map_request.find("Content-Length") != clt.map_request.end()
		&& clt.map_request["Method"] == "POST"
	&& stringToInt(clt.map_request["Content-Length"])
		> stringToInt(web.config[clt.config].max_body_size))
		return 413;
	return 0;
}

int parseRequestData(struct client &clt, struct webserv &web)
{
	int statusCode;
	if ((statusCode = isRequestWellFormed(clt, web)))
		return statusCode;

	if ((statusCode = parsLocation(clt, web)))
		return statusCode;

	std::vector<std::string>::iterator iter;
	if (clt.config != -1 && clt.location != -1)
	{
		iter = std::find(web.config[clt.config].location[clt.location].allow.begin(),
		web.config[clt.config].location[clt.location].allow.end(), clt.map_request["Method"]);
		if (iter == web.config[clt.config].location[clt.location].allow.end())
			return 405;
		return 0;
	}
	return 405;
}

int parseRequest(struct webserv &web, struct client &clt)
{
	std::cout << "\033[91m-----------------------------------------------------------\n";
	clt.response_is_ready = true;
	clt.config = -1;
	clt.location = -1;
	
	fillRequestData(clt);

	return parseRequestData(clt, web);
}

// 	// std::cout<<std::string(clt.buffer.str(), 65005)<<std::endl;
