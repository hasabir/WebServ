/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 11:40:12 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/23 21:11:30 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void	maxFd(struct webserv& web)
{
	int i;
	int j;
	int size_s;
	int	size_c;
	int size_fd;
	int max;

	max = -1;
	i = 0;
	size_s = web.servers.size();
	size_c = web.clients.size();
	while (i < size_s)
	{
		j = 0;
		size_fd = web.servers[i].socketFd.size();
		while (j < size_fd)
		{
			if (web.servers[i].socketFd[j] > max)
				max = web.servers[i].socketFd[j];
			j++;
		}
		i++;
	}
	i = 0;
	while (i < size_c)
	{
		if (web.clients[i].fd > max)
			max = web.clients[i].fd;
		i++;
	}
	web.maxReadFd = max;
}

void	receiveRequest(struct webserv& web, struct client& clt, int clt_i)
{
	char							line[100000];
	int 							i;
	int								n_byte_readed;

	memset(line, 0, 100000);
	n_byte_readed = 0;
	n_byte_readed = recv(clt.fd, line, 99999, 0);
	line[n_byte_readed] = 0;
	if (n_byte_readed < 0)
	{
		closeConnection(web, clt_i);	
		return ;
	}
	if (n_byte_readed == 0)
	{
		clt.request_is_ready = true;
		return ;
	}
	else
	 	clt.buffer.write((char *) line, n_byte_readed); //	clt.buffer << line;
	if (endOfTheRequest(clt.buffer.str(), clt.bodys) == 0)
	{
		clt.request_is_ready = true;
		FD_SET(clt.fd, &web.writes);
	}
}
