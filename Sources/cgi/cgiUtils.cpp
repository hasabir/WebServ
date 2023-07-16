/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 12:00:54 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/16 21:17:58 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

int get_time(class CGI& cgi)
{
	struct timeval current_time;
	long time;

	gettimeofday(&current_time, NULL);
	if (!cgi.time)
		cgi.time = current_time.tv_sec;
	time = current_time.tv_sec - cgi.time;
	if (time < 0)
		time *= -1;
	return (time);
}

void	fill_CGI_ENV(struct client &clt, struct webserv &web)
{
	// std::cout << "query string = " << clt.map_request["QUERY_STRING"] << std::endl;
	clt.cgi.env.push_back("QUERY_STRING=" + clt.map_request["QUERY_STRING"]);
	clt.cgi.env.push_back(std::string("REMOTE_ADDR=" + web.config[clt.config].host));
	clt.cgi.env.push_back(std::string("REQUEST_METHOD=GET"));
	clt.cgi.env.push_back(std::string("SCRIPT_NAME=") + clt.map_request["URI"]);
	clt.cgi.env.push_back("SERVER_PORT=" + clt.port);
	clt.cgi.env.push_back(std::string("SCRIPT_FILENAME=") + clt.map_request["URI"]);
	clt.cgi.env.push_back("HTTP_HOST="+clt.map_request["Host"]);
	clt.cgi.env.push_back("HTTP_USER_AGENT=" + clt.map_request["User-Agent"]);
	if(clt.map_request["Method"] == "POST")
	{
		clt.cgi.env.push_back("CONTENT_TYPE=" + clt.map_request["CONTENT-TYPE"]);
		clt.cgi.env.push_back("CONTENT_LENGTH=" + clt.map_request["CONTENT-LENGTH"]);
	}
	if (!clt.map_request["Cookie"].empty())
		clt.cgi.env.push_back("HTTP_COOKIE=" + clt.map_request["Cookie"]);
	clt.cgi.env.push_back("COUNTER=10");
}

int isCgiConfigured(struct client &clt, struct webserv &web,  std::string filePath)
{
	std::vector<std::pair<std::string, std::string> >::iterator iter;
	
	int	index = filePath.find_last_of('.');
	if (index == std::string::npos)
		return 0;
	clt.cgi.extention = filePath.substr(index, filePath.size());
	for (iter = web.config[clt.config].location[clt.location].cgi.begin();
		iter != web.config[clt.config].location[clt.location].cgi.end()
		&& iter->first != clt.cgi.extention;
		iter++);
	if (iter == web.config[clt.config].location[clt.location].cgi.end())
		return 0;
	clt.cgi.interpreter = iter->second;
	return 1;
}

std::string	parsePHPcgi(std::string fileName, std::string &header)
{
	std::ifstream	in(fileName);
	std::string 	responseFileName;
	std::string		content_type;
	std::fstream 	out;
	std::string 	line;

	if (!in.is_open())
		throw std::runtime_error("Error: opening file\n");
	while (getline(in, line,'\n') && line != "\r")
	{
		if ((line.find("Content-type")) != std::string::npos)
		{
			int start = line.find(':') + 2;
			int end = line.find(';');
			if (end == std::string::npos)
				end = line.length() - 1;
			content_type = line.substr(start, end - start);
		}
		else
			header += line + "\n";
	}
	std::map<std::string, std::string> contentTypes;
	std::map<std::string, std::string>::iterator iter;
	
	fillMapContentTypes(contentTypes);
	for (iter = contentTypes.begin(); iter != contentTypes.end(); iter++)
	{
		if (iter->second == content_type)
		{
			responseFileName = "cgi" + iter->first;
			// if (std::remove("cgi.html")< 0)
			// 	throw std::runtime_error("Error: remove");
			break;
		}
	}
	if (iter == contentTypes.end())
		responseFileName = "cgi.html";
	out.open(responseFileName, std::ios::out);
	while (getline(in, line))
		out << line << std::endl;
	out.close();
	in.close();
	return responseFileName;
}
