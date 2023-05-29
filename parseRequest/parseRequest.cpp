/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 06:56:52 by hasabir           #+#    #+#             */
/*   Updated: 2023/05/29 17:11:11 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"
#include <map>
#include <unordered_map>

unsigned long stringToInt(std::string str)
{
	long long n(0);
	std::string::iterator iter(str.begin());

	for (; iter != str.end() && std::isdigit(*iter); iter++)
		n = n * 10 + *iter - '0';
	if (iter != str.end() || str.empty())
	{
		if (*iter == 'M')
			n *= 1000000;
		else if (*iter == 'k')
			n *= 1000;
	}
	return n;
}

std::string	getRequestLine(std::string line, std::unordered_map<std::string, std::string> &map)
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

std::string	fillRequestData(struct client& clt,	std::unordered_map<std::string, std::string> &map)
{
	std::string line, key, value, requestLine;
	
	clt.file->open(clt.file_name, std::ios::in);
	if (!clt.file->is_open())
	{
		std::cout << "can't\n";
		return "";
	}
	std::getline(*clt.file, line);
	getRequestLine(line, map);
	while (getline(*clt.file, line))
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
	return requestLine;
} 
int isRequestWellFormed(std::unordered_map<std::string, std::string> map, struct client &clt,
						struct webserv &web)
{
	std::vector<std::string>::iterator port;
	std::string serverName;
	
	if ( (map["URI"].find_first_not_of(
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
	unsigned long tmp_len;
	tmp_len = clt.bodys.content_len;
	if (tmp_len >  stringToInt(web.config[i].max_body_size)) //! need to handle chunks and boundary
		return 413;
	return i * -1;
}

int parsLocation(std::unordered_map<std::string, std::string> map, struct client &clt,
						struct webserv &web, int i)
{
	//!getLocation(map, clt, web, i)
	// clt.file->seekg(0, std::ios::end);
	// std::streampos fileSize = clt.file->tellg();
	// clt.file->seekg(0, std::ios::beg);
	// std::cout << "fileSize = " << static_cast<int>(fileSize) << std::endl;
	// std::cout << "send = " << (send(clt.fd, clt.file, static_cast<int>(fileSize), 0)) << std::endl;
	// if (send(clt.fd, clt.file, static_cast<int>(fileSize), 0) == -1)
	// {
	// 	close(clt.fd);
	// 	return 404;
	// }

	for (int j = 0; j < web.config[i * -1].location.size(); j++)
	{
		for (int k = 0; k < web.config[i * -1].location[j].allow.size(); k++)
			std::cout << "allow = " << web.config[i * -1].location[j].allow[k] << std::endl;
		std::cout << "******************\n";
	}
	// if (web.config[i * -1].location)
	return 0;
}

int	parseRequestData(std::unordered_map<std::string, std::string> map, struct client &clt,
						struct webserv &web)
{
	int returnValue;
	if ((returnValue = isRequestWellFormed(map, clt, web)) > 0)
		return returnValue;
	if ((returnValue = parsLocation(map, clt, web, returnValue)))
		return returnValue;
	return 0;
}

void	parseRequest(struct webserv& web, struct client& clt)
{
	std::cout << "-----------------------------------------------------------\n";	
	std::unordered_map<std::string, std::string> map;
	fillRequestData(clt, map);
	std::cout << "\033[91m**********  " <<  parseRequestData(map, clt, web) << "  *******\033[00m\n";
	// std::cout << "max body s = " << web.config.at(5) 
        // std::cout << "Size: " << web.config.at().max_body_size <<  std::endl;
	// check listen
}

	// std::cout<<std::string(clt.buffer.str(), 65005)<<std::endl;