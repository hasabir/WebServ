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

std::string replaceLocation(std::string uri, std::string pattern, std::string root)
{
	std::string location(uri);
	int position = uri.find(pattern);
	location.replace(position, pattern.length(), root);
	return location;
}

int search(struct client &clt, struct webserv &web, int i)
{
	std::string::iterator iter;
	std::string location;

	for (int j = 0; j < web.config[i].location.size(); j++)
	{
		int found = clt.map_request["URI"].find(web.config[i].location[j].pattern);
	std::cout << "pattern = " << web.config[i].location[j].pattern << std::endl;
		if (found != std::string::npos)
			return j;
	}
	return -1;
}

int parsLocation(struct client &clt, struct webserv &web, int i)
{
	if ((clt.location = search(clt, web, i)) < 0)
	{
		if (clt.map_request["URI"] != "/" || web.config[i].root.empty())
		return send_404(clt);
		clt.map_request["URI"] = web.config[i].root;
		std::cout << "location = [" << clt.map_request["URI"] << "]\n";
		return 0;
	}
	if (!web.config[i].location[clt.location].redirect.empty())
		clt.map_request["URI"] = web.config[i].location[clt.location].redirect;
	else if (!web.config[i].location[clt.location].root.empty())
		clt.map_request["URI"] = replaceLocation(clt.map_request["URI"],
										 web.config[i].location[clt.location].pattern, 
										 web.config[i].location[clt.location].root);
	else if (!web.config[i].root.empty())
		clt.map_request["URI"] = replaceLocation(clt.map_request["URI"],
										 web.config[i].location[clt.location].pattern,
										 web.config[i].root);
	else
			return 404;
	std::cout << "location = [" << clt.map_request["URI"] << "]\n";
	return 0;
}

int parseRequestData(struct client &clt, struct webserv &web)
{
	int i, j;
	if ((i = isRequestWellFormed(clt, web)))
		return i;

	if ((j = parsLocation(clt, web, clt.config)))
		return j;

	std::vector<std::string>::iterator iter = std::find(web.config[clt.config].location[clt.location].allow.begin(),
		web.config[clt.config].location[clt.location].allow.end(), clt.map_request["Method"]);
	if (iter == web.config[clt.config].location[clt.location].allow.end())
		return 405;
	return 0;
}

void parseRequest(struct webserv &web, struct client &clt)
{
	std::cout << "-----------------------------------------------------------\n";
	clt.config = -1;
	clt.location = -1;
	clt.response_is_ready = true;
	fillRequestData(clt);
	std::cout << "\033[91m**********  " << parseRequestData(clt, web) << "  *******\033[00m\n";
	std::cout << "clt.config = " << clt.config << " | clt.location = " << clt.location << std::endl;
}

// 	// std::cout<<std::string(clt.buffer.str(), 65005)<<std::endl;