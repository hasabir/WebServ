/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayServerFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:35:32 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/08 19:18:52 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void	displayServerFile(std::vector<struct serverfile> conf)
{
	int i;

	i = 0;
	while (i < conf.size())
	{
		std::cout << "************************************************************" << std::endl;
		std::cout << "Host : " << conf[i].host << std::endl;
		{
			int j = 0;
			while (j < conf[i].listen.size())
			{
				std::cout << "listen : " << conf[i].listen[j] << std::endl;
				j++;
			}
		}
		{
			int j = 0;
			while (j < conf[i].server_name.size())
			{
				std::cout << "servername : " << conf[i].server_name[j] << std::endl;
				j++;
			}
		}
		{
			int j = 0;
			while (j < conf[i].error_page.size())
			{
				std::cout << "error_code: " << conf[i].error_page[j].first;
				std::cout << ", error_page: " << conf[i].error_page[j].second;
				std::cout << std::endl;
				j++;
			}
		}
		std::cout << "Max Body Size : " << conf[i].max_body_size << "\n";
		std::cout << "Root : " << conf[i].root << std::endl;
		std::cout << "Index : " << conf[i].index << std::endl;
		{
			int j = 0;
			while (j < conf[i].location.size())
			{
				std::cout << "-------------------" << "\n" << "Location : " << std::endl;
				std::cout << "pattern : " << conf[i].location[j].pattern << std::endl;
				std::cout << "root : " << conf[i].location[j].root << std::endl;
				std::cout << "index : " << conf[i].location[j].index << std::endl;
				std::cout << "autoindex : " << conf[i].location[j].autoindex << std::endl;
				std::cout << "upload : " << conf[i].location[j].upload << std::endl;
				std::cout << "upload_store : " << conf[i].location[j].upload_store << std::endl;
//				std::cout << "cgi : " << conf[i].location[j].cgi_ext << std::endl;
//				std::cout << "cgi_path : " << conf[i].location[j].cgi_path << std::endl;
				std::cout << "redirect : " << conf[i].location[j].redirect << std::endl;
				int k = 0;
				while (k < conf[i].location[j].error_page.size())
				{
					std::cout << "error code : " << conf[i].location[j].error_page[k].first << std::endl;
					std::cout << "error file : " << conf[i].location[j].error_page[k].second << std::endl;
					k++;
				}
				k = 0;
				while (k < conf[i].location[j].cgi.size())
				{
					std::cout << "cgi ext   : " << conf[i].location[j].cgi[k].first << std::endl;
					std::cout << "cgi prog  : " << conf[i].location[j].cgi[k].second << std::endl;
					k++;
				}
				k = 0;
				while (k < conf[i].location[j].allow.size())
				{
					std::cout << "allow : " << conf[i].location[j].allow[k] << "\n";
					k++;
				}
				j++;
			}
		}
		std::cout << "-------------------" << std::endl;
		i++;
	}
}

