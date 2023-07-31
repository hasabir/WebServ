/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deleteResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:47:18 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/29 23:32:14 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

int deletefolders(std::string *path)
{
	DIR* dir = opendir(path->c_str());
    if (dir == NULL) {
        return 403;
    }

    dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;  // Skip current and parent directories
        }

        std::string entryPath = *path + "/" + entry->d_name;
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
	std::string check_path = old_path;
    std::vector<std::string> parts;
	char* token = std::strtok((char *)old_path.c_str(), "/");
	int	flag_slash(0);
    while (token != NULL) {
        parts.push_back(token);
        token = std::strtok(NULL, "/");
    }
    if (std::remove(path->c_str()) != 0) {
        std::cerr << "Failed to remove directory: " << path << std::endl;
		return 403;
    }
    old_path = "";
    for (std::vector<std::string>::iterator it = parts.begin(); it != parts.end() - 1; ++it) {
        if(it == parts.end() - 2)
		{
            old_path += *it + "/" ;
		}
        else
		{
			if (path[0][0] == '/' && !flag_slash)
			{
				old_path += "/";
				flag_slash++;
			}
            old_path += *it + "/" ;// Append the current element to the string
		}
    }
    *path = old_path;
	return (0);
}


int	deleteResponse(struct webserv& web, struct client& clt)
{
	struct stat pathStat;
	std::string path;
	
	(void)web;
	if (stat(clt.map_request["URI"].c_str(), &pathStat) != 0)
	{
		return error(clt,404); 
	}

	int a = S_ISDIR(pathStat.st_mode);
	
	if (a == 0)
	{
		if(clt.location >= 0)
		{
			std::ofstream out2;
			std::ifstream input2;

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
			return error(clt,204);
        }
	}
	
	if (*clt.map_request["URI"].rbegin() != '/')
		return error(clt, 409);
	int i = deletefolders(&clt.map_request["URI"]);
	if(i != 0)
		return error(clt, i);
	else
		return error(clt, 204);
	return error(clt, 500);
}