/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 13:18:03 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/14 19:28:10 by hasabir          ###   ########.fr       */
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
		|| (!status && !S_ISREG(pathStat.st_mode)))
		&& statusCode != 200 && statusCode != 302)
		filePath = "www/error/"
					+ intToString(statusCode) + ".html";
	// 	filePath = "Default";
	// if (filePath == "Default")
	// 	filePath = "www/error/"
	// 				+ intToString(statusCode) + ".html";
	// std::ifstream file;
	
	// file.open(filePath.c_str(), std::ios::binary);
	// if (!file.is_open() && statusCode != 200 && statusCode != -302)
	// {
	// 	filePath = "www/error/"
	// 				+ intToString(statusCode) + ".html";
	// }
	// file.close();
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