/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:49:06 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/20 12:11:28 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"


int autoindex(struct client& clt, struct webserv &web)
{
	std::ofstream autoindex((clt.map_request["URI"] + "autoindex.html").c_str());
	DIR* directory;
	struct dirent* en;
	std::string pattern;

	(void)web;
	if (!autoindex)
	{
		std::cerr << "Failed to create autoindex.html" << std::endl;
		return error(clt, 404);
	}

	clt.response.autoindex = true;
	autoindex << "<html>\n"
				<< "<head><title>Index of " << clt.map_request["URI"] << "</title></head>\n"
				<< "<body>\n"
				<< "<h1>Index of " << clt.map_request["URI"] << "</h1>\n"
				<< "<hr>\n"
				<< "<ul>\n";

	directory = opendir(clt.map_request["URI"].c_str());
	if (directory)
	{
		if (*clt.response.uri.rbegin() != '/')
			clt.response.uri += "/";
		while ((en = readdir(directory)) != NULL)
		{
			if (!strcmp(en->d_name, "autoindex.html"))
				continue;
			autoindex	<< "<li>" << "<a href=\""
						<< "http://"
						<< clt.map_request["Host"] << clt.response.uri << en->d_name
						<< "\">"
						<< en->d_name << "</a></li>\n";
		}
		closedir(directory);
	}

	autoindex << "</ul>\n"
				<< "<hr>\n"
				<< "</body>\n"
				<< "</html>\n";

	autoindex.close();

	clt.map_request["URI"] +=  "autoindex.html";
	if (!clt.response.body)
		clt.response.statusCode = 200;
	return 0;
}

int	get(struct webserv& web, struct client& clt)
{
	struct stat pathStat;
	std::string path;
	
	// std::cout << "URI = " << clt.map_request["URI"] << std::endl;
	// std::cout << PURPLE <<"!!!!!!!!!!!!!!! execute cgi = |" << clt.cgi.extention << "|\n" << END;
	if (stat(clt.map_request["URI"].c_str(), &pathStat))
	{
		if (clt.location >= 0
			&& !web.config[clt.config].location[clt.location].redirect.empty())
		{
			if (clt.response.statusCode)
				return clt.response.statusCode;
			return clt.response.statusCode = 302;
		}
		return error(clt, 404);
	} 
	if (!S_ISDIR(pathStat.st_mode))
	{
		if (clt.location >= 0
			&& !web.config[clt.config].location[clt.location].redirect.empty())
		{
			clt.response.body = true;
			if (clt.response.statusCode)
				return clt.response.statusCode;
			return clt.response.statusCode = 302;
		}
		if (clt.location >= 0 && !web.config[clt.config].location[clt.location].cgi.empty())
		{
			if ((cgi(web, clt)))
			{
				std::cout << "########## get = |" << clt.cgi.extention << "|\n";
				return 0;
			}
		}
		return clt.response.statusCode = 200;
	}
	if (*clt.map_request["URI"].rbegin() != '/')
	{
		clt.map_request["URI"] += "/";
		clt.response.body = true;
		clt.response.statusCode = 301;
	}
	if (clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
	{
		path = clt.map_request["URI"] + web.config[clt.config].location[clt.location].index;
	}
	else if (clt.location < 0 && !web.config[clt.config].index.empty())
		path = clt.map_request["URI"] + web.config[clt.config].index;
	if (stat(path.c_str(), &pathStat))
	{
		path = clt.map_request["URI"] + "index.html";
		// return clt.response.statusCode = 200;
	}
	if (!stat(path.c_str(), &pathStat))
	{
		// std::cout << GREEN << "\nuri -> = " << path << END << std::endl;//!
		clt.map_request["URI"] = path;
		if (cgi(web, clt))
		{
			std::cout << RED << "status code = " << clt.response.statusCode << std::endl << END;
			return 0;
		}
		if (!clt.response.body)
			clt.response.statusCode = 200;
		return 0;
	}
	if ((clt.location >= 0
		&& (web.config[clt.config].location[clt.location].autoindex.empty() ||
			web.config[clt.config].location[clt.location].autoindex == "off"))
		|| clt.location < 0)
			return error(clt, 403);
	return autoindex(clt, web);
}
