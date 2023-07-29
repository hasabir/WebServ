/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsRequestHeader.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 19:50:33 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/27 15:46:52 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

std::string	getRequestLine(std::string line, std::map<std::string, std::string> &map)
{
	std::stringstream lineToParse(line);
 	std::string token;

	getline(lineToParse, token, ' ');
	map["Method"] = token;
	getline(lineToParse, token, ' ');
	map["URI"] = token;
	if (map["URI"].find("?") != std::string::npos)
	{
		std::stringstream uri(map["URI"]);
		getline(uri, map["URI"], '?');
		getline(uri, map["QUERY_STRING"], '\0');
	}
	getline(lineToParse, token, ' ');
	map["Version"] = token;
	return line;
}

void	fillRequestData(struct client& clt)
{
	std::string line, key, value, requestLine;
	clt.file->open(clt.file_name.c_str(), std::ios::in);
	if (!clt.file->is_open())
	{
		error(clt, 500);
		return ;
	}
	std::getline(*clt.file, line);
	getRequestLine(line, clt.map_request);
	clt.response.uri = clt.map_request["URI"];
	while (getline(*clt.file, line))
	{
		if (line == "\r")
			break ;
		std::stringstream lineToParse(line);
		getline(lineToParse, key, ':');
		getline(lineToParse, value, '\r');
		value.erase(0, 1);
		clt.map_request[key] = value;
	}
	clt.file->close();
	// std::map<std::string, std::string>::iterator iter;
	// int i = 0;
	// for (iter = clt.map_request.begin(); iter != clt.map_request.end();i++, iter++) {
    //     std::cout << "\033[92m" <<  iter->first << " | " << iter->second << "\033[00m\n";
    // }
	return ;
}
