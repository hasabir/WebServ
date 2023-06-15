/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:49:06 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/15 21:27:47 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


int autoindex(struct client& clt)
{
	std::ofstream autoindex((clt.map_request["URI"] + "/autoindex.html").c_str());
	DIR* directory;
	struct dirent* en;

	clt.response.autoindex = true;
	if (!autoindex)
	{
		std::cerr << "Failed to create autoindex.html" << std::endl;
		return 404;
	}

	autoindex << "<html>\n"
				<< "<head><title>Index of " << clt.map_request["URI"] << "</title></head>\n"
				<< "<body>\n"
				<< "<h1>Index of " << clt.map_request["URI"] << "</h1>\n"
				<< "<hr>\n"
				<< "<ul>\n";

	directory = opendir(clt.map_request["URI"].c_str());
	if (directory)
	{
		while ((en = readdir(directory)) != NULL)
		{
			autoindex << "<li>" << en->d_name << "</li>\n";
		}
		closedir(directory);
	}

	autoindex << "</ul>\n"
				<< "<hr>\n"
				<< "</body>\n"
				<< "</html>\n";

	autoindex.close();

	clt.map_request["URI"] +=  "/autoindex.html";
	return 200;
}



int	get(struct webserv& web, struct client& clt)
{
	struct stat pathStat;
	std::string path;

	if (stat(clt.map_request["URI"].c_str(), &pathStat) != 0)
	{
		if (clt.location >= 0
			&& !web.config[clt.config].location[clt.location].redirect.empty())
		{
			std::cout << "status code = " << clt.response.statusCode << std::endl;
			if (clt.response.statusCode)
				return clt.response.statusCode;
			return 302;
		}
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
		if (web.config[clt.config].location[clt.location].autoindex.empty() ||
			web.config[clt.config].location[clt.location].autoindex == "off")
			return 403;
	}
	return autoindex(clt);
	return 0;
}