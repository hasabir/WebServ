/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 19:45:42 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/13 15:13:52 by hasabir          ###   ########.fr       */
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
		//! else if (G)
	}
	return n;
}
std::string intToString(int n)
{
	std::string str;
	char c;
	while (n > 0)
	{
		c = (n % 10 + '0');
		str = c + str;
		n /= 10;
	}
	return str;
}

std::string replaceLocation(std::string uri, std::string pattern, std::string root)
{
	std::string location(uri);
	int position = uri.find(pattern);
	location.replace(position, pattern.length(), root);
	return location;
}
std::string host(std::string host)
{
	if (host.empty())
		return "0.0.0.0";
	if (host == "localhost")
		return "127.0.0.1";
	return host;
}

int search(struct client &clt, struct webserv &web, int i)
{
	std::string::iterator iter;
	std::string location;

	for (int j = 0; j < web.config[i].location.size(); j++)
	{
		if (web.config[i].location[j].pattern.size() == 1 && clt.map_request["URI"].size() != 1)
			continue;
		int found = clt.map_request["URI"].find(web.config[i].location[j].pattern);
		// std::cout << "pattern = " << web.config[i].location[j].pattern << std::endl;
		if (found != std::string::npos)
			return j;
	}
	return -1;
}

std::string getStatusMessage(int statusCode)
{
	std::map<int, std::string> map;
	
	map[400] = "Bad Request\r\n";
	map[403] = "Forbidden\r\n";
	map[404] = "Not Found\r\n";
	map[413] = "Content Too Large\r\n";
	map[414] = "Request-URI Too Long\r\n";
	map[501] = "Bad Request\r\n";
	map[423] = "Request Entity Too Large\r\n";
	map[405] = "Method Not Allowed\r\n";
	map[302] = "Found\r\n";
	map[301] = "Moved Permanently\r\n";
	map[200] = "OK\r\n";
	map[-302] = "Found\r\n";
	map[0] = "OK\r\n";
	return map[statusCode];
}

void fillMapContentTypes(std::map<std::string, std::string> &contentTypes)
{
	contentTypes[".html"] = "text/html";
	contentTypes[".htm"] = "text/html";
	contentTypes[".txt"] = "text/plain";
	contentTypes[".css"] = "text/css";
	contentTypes[".js"] = "text/javascript";
	contentTypes[".json"] = "application/json";
	contentTypes[".xml"] = "application/xml";
	contentTypes[".pdf"] = "application/pdf";
	contentTypes[".zip"] = "application/zip";
	contentTypes[".jpeg"] = contentTypes["jpg"] = "image/jpeg";
	contentTypes[".png"] = "image/png";
	contentTypes[".gif"] = "image/gif";
	contentTypes[".mp3"] = "audio/mpeg";
	contentTypes[".mp4"] = "video/mp4";
	contentTypes[".cpp"] = "text/x-c++src";
	contentTypes[".c"] = "text/x-csrc";
	contentTypes[".py"] = "text/x-python";
}