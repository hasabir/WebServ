/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 13:18:03 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/15 12:28:50 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

std::string getFilePath(struct client& clt, struct webserv &web, int statusCode)
{
	std::vector<std::pair<std::string, std::string> >::iterator iter;
	std::string filePath;
	struct stat pathStat;
	int status;

	if (statusCode == 200 || statusCode == -302)
		return clt.map_request["URI"];
	if (clt.location >= 0 )
	{
		for (iter = web.config[clt.config].location[clt.location].error_page.begin();
		iter != web.config[clt.config].location[clt.location].error_page.end()
		&& iter->first != intToString(statusCode); iter++);
		if (iter != web.config[clt.config].location[clt.location].error_page.end())
			filePath = iter->second;
	}
	if (clt.location < 0
		|| iter == web.config[clt.config].location[clt.location].error_page.end())
	{
		for (iter = web.config[clt.config].error_page.begin();
			iter != web.config[clt.config].error_page.end()
			&& iter->first != intToString(statusCode); iter++);
		if (iter != web.config[clt.config].error_page.end())
			filePath = iter->second;
	}
	if (iter == web.config[clt.config].error_page.end())
		filePath = "www/error/"
					+ intToString(statusCode) + ".html";
	if (((status = stat(filePath.c_str(), &pathStat))
		|| (!status && !S_ISREG(pathStat.st_mode))))
		filePath = "www/error/"
					+ intToString(statusCode) + ".html";
	return filePath;
}

std::string getContentType(std::string filePath)
{
	std::map<std::string, std::string> contentTypes;
	std::map<std::string, std::string>::iterator iter;
	std::string type;
	int index;
	
	fillMapContentTypes(contentTypes);
	index = filePath.rfind('.');
	type = filePath.substr(index, filePath.size());
	if ((iter = contentTypes.find(type)) != contentTypes.end())
		return iter->second;
	return "application/octet-stream";
}

std::string decimalToHex(int decimal)
{
  std::stringstream hex;
  hex << std::hex << decimal;
  return hex.str();
}

void	initData(struct client &clt, std::string filePath, std::ifstream &file)
{
		file.seekg(0, std::ios::end);
		clt.response.fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		clt.response.len = clt.response.fileSize;
		clt.response.sizeFrame = clt.response.len * 0.1;
		clt.response.nbrFrames = static_cast<unsigned long>(clt.response.fileSize)
								/ clt.response.sizeFrame;
		clt.response.position = file.tellg();
}