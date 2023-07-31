/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:48:15 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/30 12:03:43 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"


int post(struct webserv& web, struct client& clt)
{
	struct stat pathStat;
	std::string fileName;
	std::string destination;
	std::string source;

	if (clt.location >= 0 && web.config[clt.config].location[clt.location].upload == "on")
	{

		if (stat(web.config[clt.config].location[clt.location].upload_store.c_str(),&pathStat))
			return error(clt, 404);
		if(clt.map_request["Content-Length"].empty())
		{
			if(clt.body_length > stringToInt(web.config[clt.config].max_body_size))
				return (error(clt,413));
		}
		if(clt.upload_files.empty())
			return error(clt, 400);
		if (clt.map_request["Content-Type"].find("multipart/form-data") != std::string::npos)
			source = clt.upload_files[0].filename;
		else
		{
			std::map<std::string, std::string> map;
			fillMapContentTypes(map);
			std::map<std::string , std::string> ::iterator iter;
			
			for(iter = map.begin() ;iter != map.end() 
				&& iter->second != clt.map_request["Content-Type"]; iter++);
			if (iter == map.end())
				source = clt.upload_files[0].filename + ".txt";
			else
				source = clt.upload_files[0].filename + iter->first;
		}
		fileName = source.substr(source.find_last_of("/") + 1, source.length());
		if (*web.config[clt.config].location[clt.location].upload_store.rbegin() != '/')
			web.config[clt.config].location[clt.location].upload_store += "/";
		destination = web.config[clt.config].location[clt.location].upload_store + fileName;
		if (std::rename(clt.upload_files[0].filename.c_str(), source.c_str())
			|| std::rename(source.c_str(), destination.c_str()))
			return error(clt, 500);
		return error(clt, 201);
	}
	else
	{
		if (stat(clt.map_request["URI"].c_str(), &pathStat))
			return error(clt, 404);
		if (!S_ISDIR(pathStat.st_mode))
		{
			if (clt.location >= 0 && !web.config[clt.config].location[clt.location].cgi.empty()
				&& cgi(web, clt))
				return 0;
			return error(clt, 403);
		}
		if (*clt.map_request["URI"].rbegin() != '/')
		{
			clt.response.uri += "/";
			clt.map_request["URI"] = clt.response.uri;
			clt.response.body = false;
			return clt.response.statusCode = 308;
		}
		if (clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
			clt.map_request["URI"] += web.config[clt.config].location[clt.location].index;
		else if (!web.config[clt.config].index.empty())
			clt.map_request["URI"] += web.config[clt.config].index;
		if (!stat(clt.map_request["URI"].c_str(), &pathStat) && cgi(web, clt))
			return 0;
		return error(clt, 403);
	}
	return 0;
}