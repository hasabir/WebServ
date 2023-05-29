/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkConfigdata.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 21:03:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/29 18:23:43 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int	DuplicatePort(std::string listen, std::vector<std::string> port)
{
	int j;
	int p_size;
	int l_size;

	j = 0;
	p_size = port.size();
	while (j < p_size)
	{
		if (listen == port[j])
			return (1);
			j++;
	}
	return (0);
}

int	valideListenDirective(std::vector<std::string>& listen, std::vector<std::string>& port)
{
	int 								i;
	int 								size;
	std::vector<std::string>::iterator	it;

	// remove duplication of a port in a single server
	i = 0;
	size = listen.size();
	it = listen.begin();
	while (i < size)
	{
		int j = i + 1;
		while (j < size)
		{
			if (listen[i] == listen[j])
			{
				listen.erase(it + i);
				size = listen.size();
				i = 0;
				break;
			}
			else
				j++;
		}
		i++;
	}


	//check if a port number is valide else remove it from the server 
	//and check if each server have diferent otherwise remove this server
	//if remove duplication and unvalid ports lead to empty listen directive 
	//mean no port remain in that particulare server we remove that server
	size = listen.size();
	it = listen.begin();
	if (size == 0)
		return (1);
	i = 0;
	while (i < size)
	{
		if (validePort(listen[i]))   
		{
			if (DuplicatePort(listen[i], port) == 1)
				return (1);
			port.push_back(listen[i]);
			i++;
		}
		else
		{
			listen.erase(it + i);
			size = listen.size();
		}
	}
	if (size == 0)
		return (1);
	return (0);
}


int	validHostDirective(std::string& host)
{
	int 			i;
	int 			j;
	int 			size;
	std::string		value;
	int				nbr;
	int				dot;

	size = host.size();
	if (size == 0)
	{
		host.assign("0.0.0.0");
		return (0);
	}
	if (host == "localhost")
		return (0);
	i = 0;
	dot = 0;
	nbr = 0;
	while (i < size)
	{
		j = 0;
		while (i + j < size && (host[i + j] >= '0' && host[i + j] <= '9'))
			j++;
		if (dot < 3 && (j == 0 || j > 3 || host[i + j] != '.'))
				return (1);
		if (dot == 3 && (j == 0 || j > 3 || host[i + j] != 0))
		{
			std::cout << "here\n";
			return (1);
		}
		nbr = toInt(host.substr(i, j));
		if (nbr < 0 || nbr > 255)
			return (1);
		dot++;
		i = i + j;
		if (host[i] == '.')
			i++;
	}
	if (dot != 4)
		return (1);
	return (0);
}

int	checkConfigData(struct webserv& web)
{
	int 									i;
	int 									size;
	int 									vld;
	std::vector<std::string> 				port;
	std::vector<struct serverfile>::iterator it;

	i = 0;
	vld = 0;
	it = web.config.begin();
	size = web.config.size();
	while (i < size)
	{
		if (validHostDirective(web.config[i].host))
		{
			web.config.erase(it + i);
			it = web.config.begin();
			size = web.config.size();
		}
		else if (valideListenDirective(web.config[i].listen, port))
		{
			web.config.erase(it + i);
			it = web.config.begin();
			size = web.config.size();
		}
		else 
			i++;
	}
	//? displayServerFile(web.config);
	return (0);
}

