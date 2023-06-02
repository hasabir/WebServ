/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsRequestHeader.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 19:50:33 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/02 16:48:51 by hasabir          ###   ########.fr       */
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

std::string	fillRequestData(struct client& clt)
{
	std::string line, key, value, requestLine;
	clt.file->open(clt.file_name.c_str(), std::ios::in);
	if (!clt.file->is_open())
	{
		std::cout << "can't\n";
		return "";
	}
	std::getline(*clt.file, line);
	std::cout << "line = " << line << std::endl;
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
	std::map<std::string, std::string>::iterator iter;
	int i = 0;
	for (iter = clt.map_request.begin(); iter != clt.map_request.end();i++, iter++) {
        std::cout << "\033[92m" <<  iter->first << " | " << iter->second << "\033[00m\n";
    }
	return requestLine;
} 
int isRequestWellFormed(struct client &clt, struct webserv &web)
{
	std::vector<std::string>::iterator	port;
	std::string							serverName;
	
	int i = 0;
	for (; i < web.config.size(); i++)
	{
		port = std::find(web.config[i].listen.begin(), web.config[i].listen.end(),
				clt.map_request["Host"].substr(clt.map_request["Host"].find(":") + 1));
		if (port != web.config[i].listen.end())
		{
			std::cout << "port = " << *port << std::endl;
			break;
		}
	}
	clt.config = i;
	if ( (clt.map_request["URI"].find_first_not_of(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%")
		!= std::string::npos)
		|| (clt.map_request["Method"] == "POST" && clt.map_request.find("Transfer-Encoding") == clt.map_request.end()
		&& clt.map_request.find("Content-Length") == clt.map_request.end()))
		return 400;
	if (clt.bodys.chunks_flag && clt.map_request["Transfer-Encoding"] != "chunked")
		return 501;
	
	if (clt.map_request["URI"].size() > 2048)
		return 414;
	unsigned long tmp_len;
	tmp_len = clt.bodys.content_len;
	if (clt.map_request["Method"] == "POST" && tmp_len >  stringToInt(web.config[i].max_body_size)) //! need to handle chunks and boundary
		return 413;
	return 0;
}