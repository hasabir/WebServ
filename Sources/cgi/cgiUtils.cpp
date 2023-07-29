 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgiUtils.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/13 12:00:54 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/20 17:45:38 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

int get_time(struct client &clt)
{
	struct timeval current_time;
	long time;

	gettimeofday(&current_time, NULL);
	if (!clt.cgi.time)
		clt.cgi.time = current_time.tv_sec;
	time = current_time.tv_sec - clt.cgi.time;
	if (time < 0)
		time *= -1;
	return (time);
}

void	fill_CGI_ENV(struct client &clt, struct webserv &web)
{
	if (clt.map_request["Method"] == "GET")
		clt.cgi.env.push_back("QUERY_STRING=" + clt.map_request["QUERY_STRING"]);
	clt.cgi.env.push_back(std::string("REMOTE_ADDR=" + web.config[clt.config].host));
	clt.cgi.env.push_back(std::string("REQUEST_METHOD=") + clt.map_request["Method"]);
	clt.cgi.env.push_back(std::string("SCRIPT_NAME=") + clt.map_request["URI"]);
	clt.cgi.env.push_back("SERVER_PORT=" + clt.port);
	clt.cgi.env.push_back(std::string("SCRIPT_FILENAME=") + clt.map_request["URI"]);
	clt.cgi.env.push_back("HTTP_HOST="+clt.map_request["Host"]);
	clt.cgi.env.push_back("HTTP_USER_AGENT=" + clt.map_request["User-Agent"]);
	if(clt.map_request["Method"] == "POST")
	{
		clt.cgi.env.push_back("CONTENT_TYPE=" + clt.map_request["Content-Type"]);
		clt.cgi.env.push_back("CONTENT_LENGTH=" + clt.map_request["Content-Length"]);
	}
	if (!clt.map_request["Cookie"].empty())
		clt.cgi.env.push_back("HTTP_COOKIE=" + clt.map_request["Cookie"]);
	clt.cgi.env.push_back("COUNTER=10");
}

int isCgiConfigured(struct client &clt, struct webserv &web,  std::string filePath)
{
	std::vector<std::pair<std::string, std::string> >::iterator iter;
	
	size_t	index = filePath.find_last_of('.');
	if (index == std::string::npos)
		return 0;
	clt.cgi.extention = filePath.substr(index, filePath.size() - index);
	for (iter = web.config[clt.config].location[clt.location].cgi.begin();
		iter != web.config[clt.config].location[clt.location].cgi.end()
		&& iter->first != clt.cgi.extention;
		iter++);
	if (iter == web.config[clt.config].location[clt.location].cgi.end())
		return 0;
	clt.cgi.interpreter = iter->second;
	return 1;
}

std::string	parsePHPcgi(std::string &fileName, std::string &header, std::string suffix)
{
	std::ifstream	in(fileName.c_str());
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
			size_t start = line.find(':') + 2;
			size_t end = line.find(';');
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
			responseFileName = "cgi" + suffix + iter->first;
			if (std::remove(fileName.c_str())< 0)
				throw std::runtime_error("Error: remove");
			break;
		}
	}
	if (iter == contentTypes.end())
		responseFileName = "cgi" + suffix + ".html";
	out.open(responseFileName.c_str(), std::ios::out);
	while (getline(in, line))
		out << line << std::endl;
	out.close();
	in.close();
	fileName = responseFileName;
	return responseFileName;
}
