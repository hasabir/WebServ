/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:04:07 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/02 17:36:45 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

char    temp[2048] = "HTTP/1.0 200 OK\r\n Server: webserver-c\r\n Content-type: text/    html\r\n\r\n <html> Daba machimochkil    </html>\r\n";


void	closeConnection(struct webserv& web, int client_i)
{
   	std::vector<client>::iterator it;

	it = web.clients.begin();
	FD_CLR(web.clients[client_i].fd , &web.writes);
	close(web.clients[client_i].fd);
	while (client_i < web.clients.size() && (*it).fd != web.clients[client_i].fd && it != web.clients.end())
		it++;
	web.clients.erase(it);
	maxFd(web);
}

void	handleConnection(struct webserv& web)
{
	struct client 	newClient;
	int				i;
	int				j;
	int				size;
	int				k;
	
	size = web.servers.size();	
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			if (FD_ISSET(web.servers[i].socketFd[j], &web.tmp_read))
			{
					newClient.len = sizeof(newClient.addr);
					newClient.fd = accept(web.servers[i].socketFd[j], (struct sockaddr *)&newClient.addr, &newClient.len);
					if (newClient.fd < 0)
					{
						std::cerr << "Error : Fail connecting to client" << std::endl;
						return ;
					}
					else
					{
						FD_SET(newClient.fd, &web.reads);
						web.clients.push_back(newClient);
						k = web.clients.size() - 1;
						web.clients[k].bodys.chunks_flag = 0;
						web.clients[k].bodys.boundary_flag = 0;
						web.clients[k].bodys.content_length_flag = 0;
						web.clients[k].bodys.cr_nl_flag = 0;
						web.clients[k].bodys.chunks_len = 0;
						web.clients[k].bodys.chunks_con_len = 0;
						web.clients[k].bodys.content_len = 0;
						web.clients[k].bodys.n_chunks = 0;
						web.clients[k].bodys.cr_index = -1;
						web.clients[k].bodys.get_body_type = 0;
						web.clients[k].file_name = "req" + std::to_string(web.clients[k].fd) + ".txt";
						std::cout << "filename : " << web.clients[k].file_name << std::endl;
						maxFd(web);
						return ;
					}
			}
			j++;
		}
		i++;
	}
	size = web.clients.size();	
	i = 0;
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_read))
		{
			receiveRequest(web, web.clients[i], i);
			
			if (web.clients[i].request_is_ready == true)
			{
				FD_CLR(web.clients[i].fd , &web.reads);

				parseRequest(web, web.clients[i]);
				//handle request data;
				//std::ofstream file;
				//file.open("name.txt");
				//std::cout << web.clients[i].buffer.str();
				//file.close();	
			}
		}
		i++;
	}
	i = 0;
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_write) )
		{
			if (web.clients[i].request_is_ready == true && web.clients[i].response_is_ready == true)
			{
				send(web.clients[i].fd, temp, strlen(temp), 0);
				closeConnection(web, i);
			}
		}
		i++;
	}
}
