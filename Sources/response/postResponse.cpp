/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:48:15 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/13 08:47:17 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

std::string getContentTyp(std::string filePath)
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
	return "Not found";
}
int	post(struct webserv& web, struct client& clt)
{
	// std::string path;
	// std::string path2;
	// std::string path3;
	// bool entered = false;
	// struct stat pathStat1;
	// struct stat pathStat2;
	// struct stat pathStat3;
	// struct stat pathUpload;
	// // 	std::map<std::string, std::string>::iterator iter;
	// // int i = 0;
	// // for (iter = clt.map_request.begin(); iter != clt.map_request.end();i++, iter++) {
    // //     std::cout << "\033[92m" <<  iter->first << " | " << iter->second << "\033[00m\n";
    // // }


	// // size_t pos = web.config[clt.config].location[clt.location].pattern.rfind('/');
	// // size_t pos_first = web.config[clt.config].location[clt.location].pattern.find('/');
	// // size_t pos_temp = 0;
	// // std::string substring = "";
	// // std::string temp_substr = "";
    // // while (pos > pos_first) 
	// // {
	// // 	substring = web.config[clt.config].location[clt.location].pattern.substr(0,pos);
	// // 	pos_temp = substring.rfind('/');
	// // 	if(temp_substr.empty())
	// // 		temp_substr = web.config[clt.config].location[clt.location].pattern.substr(pos) + temp_substr;
	// // 	else
	// // 	{
	// // 		pos = substring.rfind('/');
	// // 		if(pos <= pos_first)
	// // 			break;
	// // 		temp_substr = substring.substr(pos) + temp_substr;
	// // 	}
	// // 	entered = true;
	// // }
	// // if(pos <= pos_first && entered == false)
    // //      substring = "";
		
	// // path = clt.map_request["URI"] + temp_substr;
	// if(stat(web.config[clt.config].location[clt.location].upload_store.c_str(),&pathStat1) == 0)
	// {	
	// 	if(clt.location >= 0 && web.config[clt.config].location[clt.location].upload == "on")
	// 	{
	// 		if(access(web.config[clt.config].location[clt.location].upload_store.c_str(),X_OK) == 0)
	// 		{
	// 			uploadFiles file = clt.upload_files[0];
	// 			std::ifstream input;
	// 			std::ofstream out;
	// 			if (*web.config[clt.config].location[clt.location].upload_store[web.config[clt.config].location[clt.location].upload_store.size() - 1] == "/")
	// 				web.config[clt.config].location[clt.location].upload_store[web.config[clt.config].location[clt.location].upload_store.end() - 1] = "";
	// 			std::string destination = web.config[clt.config].location[clt.location].upload_store + "/" + file.filename;
	// 			out.open(destination,std::ios::out);
	// 			if(!out.is_open())
	// 			{
	// 				std::cerr << "Error: out file\n";
	// 				exit(0);
	// 			}
	// 			input.open(file.filename,std::ios::in);
	// 			if(!input.is_open())
	// 			{
	// 				std::cerr << "Error: input file\n";
	// 				exit(0);
	// 			}
	// 			std::string line;
	// 			while(getline(input,line))
	// 			{
	// 				out<< line;
	// 			}
	// 			out.close();
	// 			input.close();
	// 			return clt.response.statusCode = 201;
	// 		}
	// 		else if(access(web.config[clt.config].location[clt.location].upload_store.c_str(),F_OK) != 0)
	// 		{
	// 			return error(clt,404);
	// 		}
	// 		else if(access(web.config[clt.config].location[clt.location].upload_store.c_str(),X_OK) != 0 )
	// 		{
	// 			return error(clt,403);
	// 		}
	// 	}	
	// }
	// // if(stat(web.config[clt.config].location[clt.location].pattern.c_str(), &pathStat3) == 0)
	// // {
	// // 	if(S_ISDIR(pathStat3.st_mode))
	// // 	{
			
	// // 		// if(clt.location >= 0 && !web.config[clt.config].location[clt.location].redirect.empty())
	// // 		// {
	// // 		// 	if(clt.response.statusCode)
	// // 		// 		return clt.response.statusCode *= -1;
	// // 		// 	clt.response.body = true;
	// // 		// 	return clt.response.statusCode = 302;
				
	// // 		// }
	// // 		if(*web.config[clt.config].location[clt.location].pattern.rbegin() == '/')
	// // 		{
	// // 			if(clt.location >= 0 && web.config[clt.config].location[clt.location].index.empty())
	// // 				return error (clt,403);
	// // 			else if(clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
	// // 			{
	// // 				path2 =web.config[clt.config].location[clt.location].pattern + web.config[clt.config].location[clt.location].index;
	// // 				if(web.config[clt.config].location[clt.location].cgi.empty())
	// // 				{
	// // 					return (error(clt,403));
	// // 				}
	// // 				else
	// // 				{
	// // 					return cgi(web,clt);
	// // 				}
	// // 			}else if(clt.location < 0 && web.config[clt.config].index.empty())
	// // 			{
	// // 				path2 =web.config[clt.config].location[clt.location].pattern + web.config[clt.config].index;
	// // 			}
	// // 		}
	// // 		if(*web.config[clt.config].location[clt.location].pattern.rbegin()	!= '/')
	// // 		{
	// // 			clt.map_request["URI"] += "/";
	// // 			if(clt.location >= 0 && !web.config[clt.config].location[clt.location].redirect.empty())
	// // 				clt.response.body = true;
	// // 			return clt.response.statusCode = 301;
	// // 		}
	// // 	}
	// // 	if (!S_ISDIR(pathStat3.st_mode))
	// // 	{
	// // 		std::cout <<"i'm here"<<std::endl;
	// // 		if(web.config[clt.config].location[clt.location].cgi.empty())
	// // 		{
	// // 			return (error(clt,403));
	// // 		}
	// // 		else
	// // 		{
	// // 			return cgi(web,clt);
	// // 		}
	// // 	}
	// // }
	// if(stat(clt.map_request["URI"].c_str(),&pathStat2) != 0)
	// 	return (error(clt,404));
	// if(S_ISDIR(pathStat2.st_mode))
	// {
		
	// 	// if(clt.location >= 0 && !web.config[clt.config].location[clt.location].redirect.empty())
	// 	// {
	// 	// 	if(clt.response.statusCode)
	// 	// 		return clt.response.statusCode *= -1;
	// 	// 	clt.response.body = true;
	// 	// 	return clt.response.statusCode = 302;
		    
	// 	// }
	// 	if(*clt.map_request["URI"].rbegin() == '/')
	// 	{
	// 		if(clt.location >= 0 && web.config[clt.config].location[clt.location].index.empty())
	// 			return error (clt,403);
	// 		else if(clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
	// 		{
	// 			path2 = clt.map_request["URI"] + web.config[clt.config].location[clt.location].index;
	// 			if(web.config[clt.config].location[clt.location].cgi.empty())
	// 			{
	// 				return (error(clt,403));
	// 			}
	// 			else
	// 			{
	// 				return cgi(web,clt);
	// 			}
	// 		}else if(clt.location < 0 && web.config[clt.config].index.empty())
	// 		{
	// 			path2 = clt.map_request["URI"] + web.config[clt.config].index;
	// 		}
	// 	}
	// 	if(*clt.map_request["URI"].rbegin() != '/')
	// 	{
	// 		clt.map_request["URI"] += "/";
	// 		if(clt.location >= 0 && !web.config[clt.config].location[clt.location].redirect.empty())
	// 			clt.response.body = true;
	// 		return clt.response.statusCode = 301;
	// 	}
	// }
	// if (!S_ISDIR(pathStat2.st_mode))
	// {
	// 	if(web.config[clt.config].location[clt.location].cgi.empty())
	// 	{
	// 		return (error(clt,403));
	// 	}
	// 	else
	// 	{
	// 		return cgi(web,clt);
	// 	}
	// }
	return clt.response.statusCode = 500;
}