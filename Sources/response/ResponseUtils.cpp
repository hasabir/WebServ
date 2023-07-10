/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseUtils.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 13:18:03 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/10 18:49:53 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

std::string getContentType(std::string filePath)
{
	std::map<std::string, std::string> contentTypes;
	std::map<std::string, std::string>::iterator iter;
	std::string type;
	int index;
	
	fillMapContentTypes(contentTypes);
	index = filePath.find_last_of('.');
	if (index == std::string::npos)
		return "application/octet-stream";
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
		clt.response.sizeFrame = 500000;
		if (clt.response.sizeFrame > clt.response.fileSize)
			clt.response.sizeFrame = clt.response.fileSize;
		// clt.response.sizeFrame = clt.response.len * 0.1;
		// clt.response.nbrFrames = static_cast<unsigned long>(clt.response.fileSize)
		// 						/ clt.response.sizeFrame;
		clt.response.nbrFrames = 1;
		clt.response.position = 0;
}