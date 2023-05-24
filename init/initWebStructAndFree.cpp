/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initWebStructAndFree.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 21:52:47 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/20 10:46:28 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


void	getServerConfig(struct webserv& web)
{
	int i;
	int size;

	i = 0;
	size = web.config.size();
	while (i < size)
	{
		struct server	serv;

		serv.serverConfig = web.config[i];
		web.servers.push_back(serv);
		int j = 0;
		while (j < serv.serverConfig.listen.size())
		{
			struct addrinfo	*info;
			web.servers[i].socket.push_back(info);
			web.servers[i].socketFd.push_back(-1);
			j++;
		}
		i++;
	}
}

void	initWebStrcut(struct webserv& web)
{
	int		i;
	int		j;
	int 	len;
	int		flag;
	int		size;

	flag = 0;
	getServerConfig(web);
	size = web.servers.size();
	memset(&web.hints, 0, sizeof(web.hints));
	web.hints.ai_family = AF_INET;
	web.hints.ai_socktype = SOCK_STREAM;
	web.hints.ai_flags = AI_PASSIVE;
	web.hints.ai_protocol = IPPROTO_TCP;
	i = 0;
	while (i < size)
	{
		j = 0;
		len = web.servers[i].serverConfig.listen.size();
		char *host = const_cast<char*>( web.servers[i].serverConfig.host.c_str());	
		while (j < len)
		{
			char *port = const_cast<char*>( web.servers[i].serverConfig.listen[j].c_str());
			web.status = 0;
			//web.status = getaddrinfo(HOST, port, &web.hints, &web.servers[i].socket[j]);
			web.status = getaddrinfo(host, port, &web.hints, &web.servers[i].socket[j]);
			if (web.status != 0)
			{
				std::cerr << "error : init web struct in port ";
				std::cout << web.servers[i].serverConfig.listen[j] << std::endl;
			}
			else
				flag++;
			j++;
		}
		i++;
	}

	if (flag == 0)
		web.status = 1;
	else
		web.status = 0;
}

void	freedWeb(struct webserv& web)
{
	int	i;
	int j;

	i = 0;
	while (i < web.servers.size())
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			close(web.servers[i].socketFd[j]);
			j++;
		}
		j = 0;
		while (j < web.servers[i].socket.size())
		{
			freeaddrinfo(web.servers[i].socket[j]);
			j++;
		}
		i++;
	}
}
