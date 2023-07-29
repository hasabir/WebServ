/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayHostPort.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:01:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/07/20 18:02:30 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	displayHostPort(struct webserv& web)
{
	struct addrinfo	*p;
	char			str[32];
	void			*addr;
	int				i;
	size_t			addr_len;

	i = 0;
	while ((unsigned int)i < web.servers.size())
	{
		int j = 0;
		while ((unsigned int)j < web.servers[i].socket.size())
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
		addr_len = sizeof(addr);
		memset(addr, 0, addr_len);
		i++;
	}
}
