/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:49:06 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/07 22:48:02 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int	get(struct webserv& web, struct client& clt)
{
	struct stat pathStat;
	std::string path;

	if (stat(clt.map_request["URI"].c_str(), &pathStat) != 0)
	{
		if (clt.location >= 0
			&& !web.config[clt.config].location[clt.location].redirect.empty())
			return sendResponse(clt, web, 302);
		return sendResponse(clt, web, 404);
	}
	if (!S_ISDIR(pathStat.st_mode))
	{
		if (!web.config[clt.config].location[clt.location].cgi_ext.empty())
			return sendResponse(clt, web, 0);
		return sendResponse(clt, web, 200);
	}
	if (*clt.map_request["URI"].rbegin() != '/')
		return sendResponse(clt, web, 301);
	if (clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
		path = clt.map_request["URI"] + web.config[clt.config].location[clt.location].index;
	else if (clt.location < 0 && !web.config[clt.config].index.empty())
		path = clt.map_request["URI"] + web.config[clt.config].index;
	if (!stat(path.c_str(), &pathStat))
	{
		clt.map_request["URI"] = path;
		return sendResponse(clt, web, 200);
	}
	path = clt.map_request["URI"] + "index.html";
	if (!stat(path.c_str(), &pathStat))
	{
		clt.map_request["URI"] = path;
		return sendResponse(clt, web, 200);
	}
	if (clt.location >= 0)
	{
		if (web.config[clt.config].location[clt.location].autoindex.empty() ||
		web.config[clt.config].location[clt.location].autoindex == "off")
			sendResponse(clt, web, 403);
	}
	//! auto index
	return sendResponse(clt, web, 0);
}