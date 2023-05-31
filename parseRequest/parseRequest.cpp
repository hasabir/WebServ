/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 06:56:52 by hasabir           #+#    #+#             */
/*   Updated: 2023/05/31 21:51:26 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

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

std::string	fillRequestData(struct client& clt)
{
	std::string line, key, value, requestLine;
	clt.file->open(clt.file_name, std::ios::in);
	std::cout << "hiiiiiiiiiiiii\n";
	if (!clt.file->is_open())
	{
		std::cout << "can't\n";
		return "";
	}
	std::getline(*clt.file, line);
	std::cout << "line = " << line << std::endl;
	getRequestLine(line, clt.map);
	while (getline(*clt.file, line))
	{
		if (line == "\r")
			break ;
		std::stringstream lineToParse(line);
		getline(lineToParse, key, ':');
		getline(lineToParse, value, '\r');
		value.erase(0, 1);
		clt.map[key] = value;
	}
	std::unordered_map<std::string, std::string>::iterator iter;
	int i = 0;
	for (iter = clt.map.begin(); iter != clt.map.end();i++, iter++) {
        std::cout << "\033[92m" <<  iter->first << " | " << iter->second << "\033[00m\n";
    }
	return requestLine;
} 
int isRequestWellFormed(struct client &clt, struct webserv &web)
{
	std::vector<std::string>::iterator port;
	std::string serverName;
	
	if ( (clt.map["URI"].find_first_not_of(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%")
		!= std::string::npos)
		|| (clt.map["Method"] == "POST" && clt.map.find("Transfer-Encoding") == clt.map.end()
		&& clt.map.find("Content-Length") == clt.map.end()))
		return 400;
	
	if (clt.bodys.chunks_flag && clt.map["Transfer-Encoding"] != "chunked")
		return 501;
	
	if (clt.map["URI"].size() > 2048)
		return 414;
	int i = 0;
	for (; i < web.config.size(); i++)
	{
		port = std::find(web.config[i].listen.begin(), web.config[i].listen.end(),
				clt.map["Host"].substr(clt.map["Host"].find(":") + 1));
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
int getLocation(struct client &clt, struct webserv &web, int i, 
				std::vector<std::pair<int, std::string> > &locations)
{
	std::string location;
	bool isRootExist(true), isRedirectExist(false);

	if ((location = web.config[i].root) == "")
		isRootExist = false;
	for (int j = 0; j < web.config[i].location.size(); j++)
	{
		if (!web.config[i].location[j].root.empty())
			location = web.config[i].location[j].root;
		if (location == "")
		{
			std::cout << "--> location = [" << location << "]" << std::endl;
			return 404;//! check error page first
		}
		location += web.config[i].location[j].pattern;
		if (!web.config[i].location[j].redirect.empty()) isRedirectExist = true;
		locations.push_back(std::make_pair(isRedirectExist, location));
	}
	return 0;
}

int parsLocation(struct client &clt,
						struct webserv &web, int i)
{
	std::vector<std::pair<int, std::string> > locations;
	int returnValue;
	
	if ((returnValue = getLocation(clt, web, i, locations)))
		return returnValue;
	
    std::vector<std::pair<int, std::string> >::iterator it;
    for (it = locations.begin(); it != locations.end(); it++) {
        std::cout << "is redirect: " << it->first << ", location: " << it->second << std::endl;
    }
	// if (web.config[i * -1].location)
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