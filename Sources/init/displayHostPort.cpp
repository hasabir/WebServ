/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayHostPort.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:01:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/29 12:50:30 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	displayHostPort(struct webserv& web)
{
	struct addrinfo	*p;
	char			str[32];
	void			*addr;
	int				i;

	i = 0;
	while (i < web.servers.size())
	{
		int j = 0;
		while (j < web.servers[i].socket.size())
		{
		p = web.servers[i].socket[j];
		while (p != NULL)
		{
			if (p->ai_family == AF_INET)
			{
				addr = &((struct sockaddr_in *) p->ai_addr)->sin_addr;
				inet_ntop(p->ai_family, addr, str, sizeof(str));
				std::cout << "HostIP4 : " << str << std::endl; 
				std::cout << "Port    : ";
				std::cout << ntohs(((struct sockaddr_in *) p->ai_addr)->sin_port) << std::endl; 
			}
			p = p->ai_next;
		}
		j++;
		}
		memset(str, 0, 32);
		memset(addr, 0, sizeof(addr));
		i++;
	}
}
