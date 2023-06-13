/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:43:10 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/08 19:18:52 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


int	parseConfigData(std::vector<std::string>& file)
{
	removeEmptyLineAndComments(file);//remove empty lines and comment from config file
	moveBracketsToNextLine(file);//only brackets thats come after server or location block
	splitSemiColons(file);//if there is multiple directive in the same line move it to next one
	removeEmptySemiColons(file);// remove lines that have only semi colons
	if (valideServersBlock(file) == 0)	//check also that there is no data outside servers 
	{
		std::cerr << "Data outside server block:" << std::endl;
		return (1);
	}
	return (0);
}

void	parseConfigFile(struct webserv& web, int ac, char **av)
{
	std::ifstream				file;
	const char 					*filename;
	std::string					line;
	std::vector<std::string>	conf_file;//fill in with line from server.conf
	int ports;
	int i;

	if (ac == 2)
		filename = av[1];
	else
		filename = CONFIGFILE;
	file.open(filename);
	if (file.is_open() == false)
	{
		std::cerr << "Error opening config file" << std::endl;
		web.status = 1;
		return ;
	}
	while (getline(file, line))
	{
		conf_file.push_back(line);
	}
	file.close();
	if (conf_file.size() == 0)
	{
		std::cerr << "Error Empty file" << std::endl;
		web.status = 1;
		return ;
	}
	if (parseConfigData(conf_file))
	{
		std::cerr << "Error in config file" << std::endl;
		web.status = 1;
		return;
	}
	getConfigData(web, conf_file);
	checkConfigData(web);
	ports = 0;
	i = 0;
	while (i < web.config.size())
	{
		ports = ports + web.config[i].listen.size();
		i++;
	}
	if (ports == 0)
		web.status = 1;
}
