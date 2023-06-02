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


int getLocation(struct client &clt, struct webserv &web, int i, std::string &location)
{
	int count(0), length, stock_count(-1);
	std::string stock_location;

	for (int j = 0; j < web.config[i].location.size(); j++)
	{
		location = web.config[i].root;
		if (!web.config[i].location[j].root.empty())
			location = web.config[i].location[j].root;
		if (location == "")
		{
			std::cout << "--> location = [" << location << "]" << std::endl;
			return -404;
		}
		length = std::min(clt.map["URI"].length(), web.config[i].location[j].pattern.length());
		if (clt.map["URI"].length() != 1)
			location += web.config[i].location[j].pattern;
		for (count = 0; count < length && clt.map["URI"][count] == web.config[i].location[j].pattern[count]
			; count++);
		std::cout << "stock location = " << stock_location << std::endl;
		if (count > stock_count || (length == 1 && web.config[i].location[j].pattern == clt.map["URI"]))
		{
			if (web.config[i].location[j].redirect.empty())
				stock_location = location;
			else
				location = web.config[i].location[j].redirect;
			if (length == 1 && web.config[i].location[j].pattern == clt.map["URI"])
				return 1;
			stock_count = count;
		}
	}
	if (clt.map["URI"].length() == 1)
	{
		location = web.config[i].root;
		if (!web.config[i].location[0].root.empty())
			location = web.config[i].location[0].root;
		if (!web.config[i].location[0].redirect.empty())
				location = web.config[i].location[0].redirect;
	}
	return stock_count;
}

void replaceSubstring(std::string& str, const std::string& oldSubStr, const std::string& newSubStr) {
	size_t pos = 0;
	while ((pos = str.find(oldSubStr, pos)) != std::string::npos) {
		str.replace(pos, oldSubStr.length(), newSubStr);
		pos += newSubStr.length();
	}
}

int search(struct client &clt, struct webserv &web, int i, bool isRedirectExist)
{
	std::string::iterator iter;
	std::string location;

	for (int j = 0; j < web.config[i].location.size(); j++)
	{
		int found = clt.map["URI"].find(web.config[i].location[j].pattern);
		if (found != std::string::npos)
		{
			std::cout << "pattern = " << web.config[i].location[j].pattern << std::endl;
			return j;
		}
	}
	return -1;
}

int parsLocation(struct client &clt,
						struct webserv &web, int i)
{
	bool isRedirectExist(false);
	std::string location;
	int returnValue;

	if (web.config[i].location.empty())
	{
		location = web.config[i].root;
		if (location.empty())
			return 404;
	}
	if ((returnValue =  search(clt, web, i, isRedirectExist)) < 0)
	{
		location = 
	}
	std::cout << "location = [" << location << "] | count = "<< returnValue << "\n";
	return 0;
}

int	parseRequestData(struct client &clt,  struct webserv &web)
{
	int returnValue;
	if ((returnValue = isRequestWellFormed(clt, web)) > 0)
		return returnValue;

	if ((returnValue = parsLocation(clt, web, returnValue * -1)))
		return returnValue;
	return 0;
}

void	parseRequest(struct webserv& web, struct client& clt)
{
	std::cout << "-----------------------------------------------------------\n";	
	fillRequestData(clt);
	std::cout << "\033[91m**********  " <<  parseRequestData(clt, web) << "  *******\033[00m\n";
	// std::cout << "max body s = " << web.config.at(5) 
        // std::cout << "Size: " << web.config.at().max_body_size <<  std::endl;
	// check listen
}

 // 	// std::cout<<std::string(clt.buffer.str(), 65005)<<std::endl;