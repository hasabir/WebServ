/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsRequestHeader.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 19:50:33 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/12 18:15:02 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

std::string	getRequestLine(std::string line, std::map<std::string, std::string> &map)
{
	std::stringstream lineToParse(line);
 	std::string token;

	getline(lineToParse, token, ' ');
	map["Method"] = token;
	getline(lineToParse, token, ' ');
	map["URI"] = token;
	getline(lineToParse, token, ' ');
	map["Version"] = token;
	return line;
}

void	fillRequestData(struct client& clt)
{
	std::string line, key, value, requestLine;
	// std::cout << "clt.file_name = " <<  clt.file_name << std::endl;
	clt.file->open(clt.file_name.c_str(), std::ios::in);
	if (!clt.file->is_open())
	{
		std::cout << "i am suppose to be here in case of an error\n";
		return ;
	}
	std::getline(*clt.file, line);
	// std::cout << "line = " << line << std::endl;
	getRequestLine(line, clt.map_request);
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
	// std::map<std::string, std::string>::iterator iter;
	// int i = 0;
	// for (iter = clt.map_request.begin(); iter != clt.map_request.end();i++, iter++) {
    //     std::cout << "\033[92m" <<  iter->first << " | " << iter->second << "\033[00m\n";
    // }
	// std::cout << "request line = " << requestLine << std::endl;
	return ;
}
