/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 06:56:52 by hasabir           #+#    #+#             */
/*   Updated: 2023/05/24 16:12:26 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"
#include <map>
#include <unordered_map>

void	getRequestLine(std::string line, std::unordered_map<std::string, std::string> &map)
{
	std::stringstream lineToParse(line);
	std::string token;

	getline(lineToParse, token, ' ');
	map["Method"] = token;
	getline(lineToParse, token, ' ');
	map["URI"] = token;
	getline(lineToParse, token, ' ');
	map["Version"] = token;
}

void	fillRequestData2(struct client& clt,	std::unordered_map<std::string, std::string> &map)
{
	std::string line, key, value;
	
	getline(clt.buffer, line);
	getRequestLine(line, map);
	while (getline(clt.buffer, line))
	{
		if (line == "\r")
			break ;
		std::stringstream lineToParse(line);
		getline(lineToParse, key, ':');
		getline(lineToParse, value, '\r');
		value.erase(0, 1);
		map[key] = value;
	}
	std::unordered_map<std::string, std::string>::iterator iter;
	int i = 0;
	for (iter = map.begin(); iter != map.end();i++, iter++) {
        std::cout << "\033[92m" <<  iter->first << " | " << iter->second << "\033[00m\n";
    }
}
int	parseRequestData(std::unordered_map<std::string, std::string> map, struct client &clt,
						struct webserv &web)
{
	std::vector<std::string>::iterator port;
	
	if (((clt.bodys.chunks_flag || clt.bodys.boundary_flag || clt.bodys.content_length_flag)
		&& map["Method"] == "GET") //! No need to be handled
		|| (map["URI"].find_first_not_of(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%")
		!= std::string::npos)
		|| (map["Method"] == "POST" && map.find("Transfer-Encoding") == map.end()
		&& map.find("Content-Length") == map.end()))
		return 400;
	
	if (clt.bodys.chunks_flag && map["Transfer-Encoding"] != "chunked")
		return 501;
	
	if (map["URI"].size() > 2048)
		return 414;
	int i = 0;
	for (; i < web.config.size(); i++)
	{
		port = std::find(web.config[i].listen.begin(), web.config[i].listen.end(),
				map["Host"].substr(map["Host"].find(":") + 1));
		if (port != web.config[i].listen.end())
		{
			std::cout << "port = " << *port << std::endl;
			break;
		}
	}
	// if ()
    std::cout << "\nServer " << i << " - Max Body Size: " << web.config[i].max_body_size << std::endl;
	//? check more than one specifier of body lengh 
	return 0;
}

void	parseRequest(struct webserv& web, struct client& clt)
{
	
	// std::cout<<std::string(clt.buffer.str(), 65005)<<std::endl;
	std::unordered_map<std::string, std::string> map;
	std::cout << "\033[94m" << clt.buffer.str() << "\033[00m" << std::endl;
	fillRequestData2(clt, map);
	std::cout << "\033[91m**********  " <<  parseRequestData(map, clt, web) << "  *******\033[00m\n";
	// std::cout << "max body s = " << web.config.at(5) 
        // std::cout << "Size: " << web.config.at().max_body_size <<  std::endl;
	// check listen
}
