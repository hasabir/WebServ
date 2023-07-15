/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:47:18 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/15 10:34:14 by hasabir          ###   ########.fr       */
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
            if (std::remove(entryPath.c_str()) != 0) {
                std::cerr << "Failed to remove file: " << entryPath << std::endl;
				return 500;
            }
        }
    }
    closedir(dir);
    // std::cout <<path<<std::endl;
    std::string old_path = *path;
    std::vector<std::string> parts;
    // std::cout << old_path << "   l"<<std::endl;
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
    sleep(1);
    *path = old_path;
    std::cout << "path :"<< *path<<std::endl;
	return (0);
}

// int deletefiles(std::string *path)
// {
//     DIR* dir = opendir(path->c_str());
//     if (dir == NULL) {
//         std::cerr << "Failed to open directory: " << *path << std::endl;
//         return 403;
//     }

//     dirent* entry;
//     while ((entry = readdir(dir)) != NULL) {
//         if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
//             continue;  // Skip current and parent directories
//         }

//         std::string entryPath = *path + "/" + entry->d_name;
//         std::cout << entryPath <<std::endl;
//         if (entry->d_type == DT_DIR) {
//             deletefolders((&entryPath));  // Recursive call for subdirectory
//         } else {
// 			if()
// 			{
//             	if (std::remove(entryPath.c_str()) != 0) {
//                 	std::cerr << "Failed to remove file: " << entryPath << std::endl;
// 					return 500;
//             	}
// 			}
//         }
//     }
//     closedir(dir);
// }

int	deleteResponse(struct webserv& web, struct client& clt)
{
	struct stat pathStat;
	std::string path;
	
	// std::cout << "URI = " << clt.map_request["URI"] << std::endl;

	if (stat(clt.map_request["URI"].c_str(), &pathStat) != 0)
	{
		return error(clt,404); 
	} 
	if (!S_ISDIR(pathStat.st_mode))
	{
		if (clt.location >= 0 && !web.config[clt.config].location[clt.location].cgi.empty())
		{
			if (cgi(web, clt) == 200)
				return 200;
		}
		else if(web.config[clt.config].location[clt.location].cgi.empty())
		{
			if (std::remove(clt.map_request["URI"].c_str()) != 0){
                std::cerr << "Failed to remove file: " << clt.map_request["URI"].c_str() << std::endl;
				return 403;
            }
			return 204;
		}
	}
	if (*clt.map_request["URI"].rbegin() != '/')
	{
		return clt.response.statusCode = 409;
	}
	if (clt.location >= 0 && !web.config[clt.config].location[clt.location].cgi.empty() && web.config[clt.config].location[clt.location].index.empty())
	{
		return clt.response.statusCode = 403;
	}
	if (clt.location >= 0 && !web.config[clt.config].location[clt.location].cgi.empty()  && !web.config[clt.config].index.empty())
	{
		if (cgi(web, clt) == 200)
			return 200;
	}
	// if (stat(path.c_str(), &pathStat))
	// 	path = clt.map_request["URI"] + "index.html";
	while(clt.map_request["URI"] != web.config[clt.config].location[clt.location].root)
	{
		int a = deletefolders(&clt.map_request["URI"]);
		if(a != 0)
			return clt.response.statusCode = a;
	}
	if(clt.map_request["URI"] == web.config[clt.config].location[clt.location].root)
		return clt.response.statusCode = 204;
	// if (!stat(path.c_str(), &pathStat))
	// {
	// 	clt.map_request["URI"] = path;
	// 	// if (!clt.response.body)
	// 		clt.response.statusCode = 200;
	// 	return 0;
	// }
	// if (clt.location >= 0)
	// {
	// 	if (web.config[clt.config].location[clt.location].autoindex.empty() ||
	// 		web.config[clt.config].location[clt.location].autoindex == "off")
	// 		return error(clt, 403);
	// }
	// return autoindex(clt, web);
	std::cout << "there is a problem \n";
	return clt.response.statusCode = 500;
}