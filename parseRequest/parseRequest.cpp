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

int search(struct client &clt, struct webserv &web, int i, bool isRedirectExist)
{
	std::string::iterator iter;
	std::string location;

	for (int j = 0; j < web.config[i].location.size(); j++)
	{
		int found = clt.map["URI"].find(web.config[i].location[j].pattern);
		if (found != std::string::npos)
			return j;
	}
	return -1;
}

int parsLocation(struct client &clt,
				 struct webserv &web, int i)
{
	bool isRedirectExist(false);
	std::string location;
	int j;

	if ((j = search(clt, web, i, isRedirectExist)) < 0)
	{
		if (clt.map["URI"] != "/" || web.config[i].root.empty())
			return 404;
		clt.map["URI"] = web.config[i].root;
		std::cout << "location = [" << location << "]\n";
		return 0;
	}
	if (!web.config[i].location[j].redirect.empty())
		clt.map["URI"] = web.config[i].location[j].redirect;
	else if (!web.config[i].location[j].root.empty())
		clt.map["URI"] = replaceLocation(clt.map["URI"],
										 web.config[i].location[j].pattern, web.config[i].location[j].root);
	else if (!web.config[i].root.empty())
		clt.map["URI"] = replaceLocation(clt.map["URI"],
										 web.config[i].location[j].pattern, web.config[i].root);
	else
		return 404;
	std::cout << "location = [" << location << "]\n";
	return -j;
}

int parseRequestData(struct client &clt, struct webserv &web)
{
	int i, j;
	if ((i = isRequestWellFormed(clt, web)) > 0)
		return i;

	if ((j = parsLocation(clt, web, i * -1)) > 0)
		return j;

	std::vector<std::string>::iterator iter = std::find(web.config[-i].location[-j].allow.begin(), web.config[-i].location[-j].allow.end(), clt.map["Method"]);
	if (iter == web.config[-i].location[-j].allow.end())
		return 405;
	return 0;
}

void parseRequest(struct webserv &web, struct client &clt)
{
	std::cout << "-----------------------------------------------------------\n";
	fillRequestData(clt);
	std::cout << "\033[91m**********  " << parseRequestData(clt, web) << "  *******\033[00m\n";
}

// 	// std::cout<<std::string(clt.buffer.str(), 65005)<<std::endl;