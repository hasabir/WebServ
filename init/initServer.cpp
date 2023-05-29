/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:12:36 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/29 12:55:19 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

/*int	initServer(struct webserv& web)
{
	struct addrinfo	*ptr;
	int				valide;
	int				status;
	int 			reuse;
	int				i;
	int				connected_socket;

	reuse = 1;//set address to be reuse 
	status = 0;
	i = 0;
	connected_socket = 0;
	while (i < MAX_PORT)
	{
		ptr = web.server[i];
		while (ptr != NULL)
		{
			valide = 0;
			//create socket listening to localhost in ipv4 and ipv6
			web.socketFd[i] = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (web.socketFd[i] > 0)
			{
				valide++;
				//set file descriptor of the socket to non-blocking
				int flags = fcntl(web.socketFd[i], F_GETFL, 0);
				//printf("stt : %d %d %d\n", flags, O_NONBLOCK, (flags & O_NONBLOCK) == O_NONBLOCK);
				status = fcntl(web.socketFd[i], F_SETFL, flags & O_NONBLOCK);
				if (status < 0)
					close(web.socketFd[i]);
				else
				{
				//	printf("stt : %d %d %d %d\n", valide, status, O_NONBLOCK, (status & O_NONBLOCK) == O_NONBLOCK);
					valide++;
					//set bind to reuse the some local address even if she is in Time Wait mode
					status = setsockopt(web.socketFd[i], SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
					if (status < 0)
						close(web.socketFd[i]);
					else
					{
						valide++;
						//Bind socket to localhost address stored in web.server for both version
						status = bind(web.socketFd[i], ptr->ai_addr, ptr->ai_addrlen);
						if (status < 0)
							close(web.socketFd[i]);
						else
						{
							valide++;
							//Turn sockets to passive mode (listen to incamming connection) max queue 5
							status = listen(web.socketFd[i], MAX_CONNECTION);
							if (status < 0)
								close(web.socketFd[i]);
							else
								valide++;
						}
					}
				}
			}
			if (valide == 5)
				connected_socket++;
			ptr = ptr->ai_next;
		}
		i++;
	}
	if (connected_socket == MAX_PORT)
		return (0);
	return (1);
}*/

int	initServer(struct webserv& web)
{
	struct addrinfo	*ptr;
	int				valide;
	int				status;
	int 			reuse;
	int				i;
	int				fd;
	int				j;
	int				size;
	int				len;
	int				connected_socket;

	reuse = 1;//set address to be reuse 
	status = 0;
	i = 0;
	connected_socket = 0;
	fd = -1;
	size = web.servers.size(); 
	while (i < size)
	{
		j = 0;
		len = web.servers[i].socket.size();
		while (j < len)
		{
			ptr = web.servers[i].socket[j];
			while (ptr != NULL)
			{
				valide = 0;
				//create socket listening to localhost in ipv4
				web.servers[i].socketFd[j] = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (web.servers[i].socketFd[j] > 0)
				{
					valide++;
					//set file descriptor of the socket to non-blocking
					int flags = fcntl(web.servers[i].socketFd[j], F_GETFL, 0);
					//printf("stt : %d %d %d\n", flags, O_NONBLOCK, (flags & O_NONBLOCK) == O_NONBLOCK);
					status = fcntl(web.servers[i].socketFd[j], F_SETFL, flags & O_NONBLOCK);
					if (status < 0)
						close(web.servers[i].socketFd[j]);
					else
					{
					//	printf("stt : %d %d %d %d\n", valide, status, O_NONBLOCK, (status & O_NONBLOCK) == O_NONBLOCK);
						valide++;
						//set bind to reuse the some local address even if she is in Time Wait mode
						status = setsockopt(web.servers[i].socketFd[j], SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
						if (status < 0)
							close(web.servers[i].socketFd[j]);
						else
						{
							valide++;
							//Bind socket to localhost address stored in web.server 
							status = bind(web.servers[i].socketFd[j], ptr->ai_addr, ptr->ai_addrlen);
							if (status < 0)
								close(web.servers[i].socketFd[j]);
							else
							{
								valide++;
								//Turn sockets to passive mode (listen to incamming connection) max queue 5
								status = listen(web.servers[i].socketFd[j], MAX_CONNECTION);
								if (status < 0)
									close(web.servers[i].socketFd[j]);
								else
									valide++;
							}
						}
					}
				}
				if (valide == 5)
				{
					std::cout << "listen to port number ";
					std::cout << web.servers[i].serverConfig.listen[j] << " is established" << std::endl;
					connected_socket++;
					while (ptr)
						ptr = ptr->ai_next;
				}
				else
					ptr = ptr->ai_next;
			}
			j++;
		}
		i++;
	}
	web.nbr_of_connections = connected_socket;
	if (connected_socket)
		return (0);
	return (1);
}
