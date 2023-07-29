/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activeSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:31:49 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/29 12:50:30 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	clearFdTmp(struct webserv& web)
{
	unsigned long		i;
	unsigned long		j;
	unsigned long		size;

	i = 0;
	size = web.servers.size();
	while (i < size)
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			if (FD_ISSET(web.servers[i].socketFd[j] , &web.tmp_read))
				FD_CLR(web.servers[i].socketFd[j] , &web.tmp_read);	
			if (FD_ISSET(web.servers[i].socketFd[j] , &web.tmp_write))
				FD_CLR(web.servers[i].socketFd[j] , &web.tmp_write);	
			j++;
		}
		i++;
	}
	i = 0;
	size = web.clients.size();
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd , &web.tmp_read))
			FD_CLR(web.clients[i].fd , &web.tmp_read);	
		if (FD_ISSET(web.clients[i].fd , &web.tmp_read))
			FD_CLR(web.clients[i].fd , &web.tmp_write);	
		i++;
	}
	FD_ZERO(&web.tmp_read);
	FD_ZERO(&web.tmp_write);
}

void	activeReadSocket(struct webserv& web)
{
	int	i;
	int	j;
	int size;

	size = web.servers.size();
	i = 0;
	while (i < size)
	{
		j = 0;
		while ((unsigned int)j < web.servers[i].socketFd.size())
		{
			FD_SET(web.servers[i].socketFd[j], &web.reads);
			j++;
		}
		i++;
	}
}

void	activeWriteSocket(struct webserv& web)
{
	int	i;
	int	j;
	int size;

	size = web.servers.size();
	i = 0;
	while (i < size)
	{
		j = 0;
		while ((unsigned int)j < web.servers[i].socketFd.size())
		{
			FD_SET(web.servers[i].socketFd[j], &web.writes);
			j++;
		}
		i++;
	}
}

void	activeSocket(struct webserv& web)
{
	int	i;
	int	j;
	int	maxFd;
	int size;

	FD_ZERO(&web.reads);
	FD_ZERO(&web.writes);
	activeReadSocket(web);
	activeWriteSocket(web);
	i = 0;
	maxFd = -1;
	size = web.servers.size();
	i = 0;
	while (i < size)
	{
		j = 0;
		while ((unsigned int)j < web.servers[i].socketFd.size())
		{
			if (web.servers[i].socketFd[j] > maxFd)
				maxFd = web.servers[i].socketFd[j] ;
			j++;
		}
		i++;
	}
	web.maxReadFd = maxFd;
}
