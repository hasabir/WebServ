/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:49:06 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/13 20:25:37 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

// int sendIndex()

int	get(struct webserv& web, struct client& clt)
{
	struct stat pathStat;
	std::string path;

	if (stat(clt.map_request["URI"].c_str(), &pathStat) != 0)
	{
		if (clt.location >= 0
			&& !web.config[clt.config].location[clt.location].redirect.empty())
			return 302;
		return 404;
	} 
	if (!S_ISDIR(pathStat.st_mode))
	{
		if (clt.location >= 0
			&& !web.config[clt.config].location[clt.location].redirect.empty())
			return -302;
		if (clt.location >= 0 && !web.config[clt.config].location[clt.location].cgi.empty())
			return 0;
		return 200;
	}
	if (*clt.map_request["URI"].rbegin() != '/')
		return 301;
	if (clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
		path = clt.map_request["URI"] + web.config[clt.config].location[clt.location].index;
	else if (clt.location < 0 && !web.config[clt.config].index.empty())
		path = clt.map_request["URI"] + web.config[clt.config].index;
	if (stat(path.c_str(), &pathStat))
		path = clt.map_request["URI"] + "index.html";

	if (!stat(path.c_str(), &pathStat))
	{
		clt.map_request["URI"] = path;
		return 200;
	}
	if (clt.location >= 0)
	{
		// std::cout << "auto index = " << web.config[clt.config].location[clt.location].autoindex << std::endl;
		if (web.config[clt.config].location[clt.location].autoindex.empty() ||
			web.config[clt.config].location[clt.location].autoindex == "off")
			return 403;
	}
	//! auto index
	return 0;
}