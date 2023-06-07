/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 06:56:52 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/01 22:54:14by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int parsLocation(struct client &clt, struct webserv &web, int i)
{
	if ((clt.location = search(clt, web, i)) < 0)
	{
		if (clt.map_request["URI"] != "/" || web.config[i].root.empty())
			return sendResponse(clt, web, 404);
		clt.map_request["URI"] = web.config[i].root;
		std::cout << "location = [" << clt.map_request["URI"] << "]\n";
		return 0;
	}
	if (!web.config[i].location[clt.location].redirect.empty())
	{
		clt.map_request["URI"] = web.config[i].location[clt.location].redirect;
		char whitSpace[] = {'\t', ' ', '\v'};
		if (std::find_first_of(clt.map_request["URI"].begin(), clt.map_request["URI"].end(),
				std::begin(whitSpace), std::end(whitSpace)) != clt.map_request["URI"].end())
		{
			std::stringstream redirect(clt.map_request["URI"]);
			std::string statusCode;

			redirect >> statusCode;
			redirect >> clt.map_request["URI"];
			std::cout << "location = [" << clt.map_request["URI"] << "]\n";
			return sendResponse(clt, web, stringToInt(statusCode));
		}
	}
	else if (!web.config[i].location[clt.location].root.empty())
		clt.map_request["URI"] = replaceLocation(clt.map_request["URI"],
										 web.config[i].location[clt.location].pattern, 
										 web.config[i].location[clt.location].root);
	else if (!web.config[i].root.empty())
		clt.map_request["URI"] = replaceLocation(clt.map_request["URI"],
										 web.config[i].location[clt.location].pattern,
										 web.config[i].root);
	else
		return sendResponse(clt, web, 404);
	std::cout << "location = [" << clt.map_request["URI"] << "]\n";
	return 0;
}

int parseRequestData(struct client &clt, struct webserv &web)
{
	int error;
	if ((error = isRequestWellFormed(clt, web)))
		return error;

	if ((error = parsLocation(clt, web, clt.config)))
		return error;

	std::vector<std::string>::iterator iter = std::find(web.config[clt.config].location[clt.location].allow.begin(),
		web.config[clt.config].location[clt.location].allow.end(), clt.map_request["Method"]);
	if (iter == web.config[clt.config].location[clt.location].allow.end())
		return sendResponse(clt, web, 405);
	return 200;
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