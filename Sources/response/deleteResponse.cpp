/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:47:18 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/20 18:03:08 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

int deletefolders(std::string *path)
{
    DIR* dir = opendir(path->c_str());
    if (dir == NULL) {
        std::cerr << "Failed to open directory: " << *path << std::endl;
        return 403;
    }

    dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;  // Skip current and parent directories
        }

        std::string entryPath = *path + "/" + entry->d_name;
        std::cout << entryPath <<std::endl;
        if (entry->d_type == DT_DIR) {
            deletefolders((&entryPath));  // Recursive call for subdirectory
        } else {
			if (access(entryPath.c_str(), W_OK) == 0) 
			{
       			if (std::remove(entryPath.c_str()) != 0){
                std::cerr << "Failed to remove file: " << entryPath.c_str() << std::endl;
				return 500;
				}
    		} else {
       			 return 403;
    		}
        }
    }
    closedir(dir);

    std::string old_path = *path;
    std::vector<std::string> parts;
    
	char* token = std::strtok((char *)old_path.c_str(), "/");
    while (token != NULL) {
        parts.push_back(token);
        token = std::strtok(NULL, "/");
    }
    if (std::remove(path->c_str()) != 0) {
        std::cerr << "Failed to remove directory: " << path << std::endl;
		return 403;
    }
    old_path = "";
    std::cout << *path<<std::endl;
    for (std::vector<std::string>::iterator it = parts.begin(); it != parts.end() - 1; ++it) {
        if(it == parts.end() - 2)
            old_path += *it ;
        else
            old_path += *it + "/" ;// Append the current element to the string
    }
    std::cout << old_path<<std::endl;
    *path = old_path;
    std::cout << "path :"<< *path<<std::endl;
	return (0);
}


int	deleteResponse(struct webserv& web, struct client& clt)
{
	struct stat pathStat;
	std::string path;
	
	std::cout << web.config[clt.config].location[clt.location].cgi.empty()<< "   "<< clt.map_request["URI"].c_str() << " result"<<std::endl;
	if (stat(clt.map_request["URI"].c_str(), &pathStat) != 0)
	{
		return error(clt,404); 
	}

	std::cout << S_ISDIR(pathStat.st_mode) << "   not a directory"<< std::endl;
	int a = S_ISDIR(pathStat.st_mode);
	
	if (a == 0)
	{
		std::cout << "Here"<< std::endl;
		if(clt.location >= 0)
		{
			std::cout << "I'm here "<<std::endl;
			std::ofstream out2;
			std::ifstream input2;
			out2.open("./www/rspnse1.html",std::ios::out);
			if(!out2.is_open())
			{
				std::cerr << "Error: out file\n";
				return  error(clt, 500);
			}
			input2.open(clt.map_request["URI"],std::ios::in);
			if(!input2.is_open())
			{
				std::cerr << "Error: input file\n";
				return  error(clt, 500);
			}
			std::string line2;
			std::string all2 = "";
			while(getline(input2,line2))
			{
				all2 += line2;
				all2 += "\n";
			}
			out2 << all2;
			out2.close();
			input2.close();
			if (access(clt.map_request["URI"].c_str(), W_OK) == 0) {
       			if (std::remove(clt.map_request["URI"].c_str()) != 0){
                std::cerr << "Failed to remove file: " << clt.map_request["URI"].c_str() << std::endl;
				return error(clt,403);
			}
			} else {
       			 return error(clt,403);
    		}

			clt.map_request["URI"] = "./www/rspnse1.html";
			return clt.response.statusCode = 204;
        }
	}
	
	if (*clt.map_request["URI"].rbegin() != '/')
	{
		return clt.response.statusCode = 409;
	}
	// if (clt.location >= 0 && !web.config[clt.config].location[clt.location].cgi.empty() && web.config[clt.config].location[clt.location].index.empty())
	// {
	// 	return clt.response.statusCode = 403;
	// }
	// if (clt.location >= 0 && !web.config[clt.config].location[clt.location].cgi.empty()  && !web.config[clt.config].index.empty())
	// {
	// 	if (cgi(web, clt) == 200)
	// 		return 200;
	// }
	
	while(clt.map_request["URI"] != web.config[clt.config].location[clt.location].root)
	{
		std::ofstream out2;
		// std::ifstream input2;
		out2.open("./www/rspnse1.html",std::ios::out);
		if(!out2.is_open())
		{
			std::cerr << "Error: out file\n";
			return  error(clt, 500);
		}
		// input2.open(clt.map_request["URI"],std::ios::in);
		// if(!input2.is_open())
		// {
		// 	std::cerr << "Error: input file\n";
		// 	return  error(clt, 500);
		// }
		// std::string line2;
		// std::string all2 = "";
		// while(getline(input2,line2))
		// {
		// 	all2 += line2;
		// 	all2 += "\n";
		// }
		out2 << clt.map_request["URI"];
		out2.close();
		// input2.close();
		int a = deletefolders(&clt.map_request["URI"]);
		if(a != 0)
			return clt.response.statusCode = a;
	}
	if(clt.map_request["URI"] == web.config[clt.config].location[clt.location].root)
	{
		clt.map_request["URI"] = "./www/rspnse1.html";
		return clt.response.statusCode = 204;
	}
	std::cout << "there is a problem \n";
	return clt.response.statusCode = 500;
}