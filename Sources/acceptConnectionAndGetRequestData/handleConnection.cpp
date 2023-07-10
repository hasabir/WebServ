/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:04:07 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/07/10 20:08:13 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	closeConnection(struct webserv& web, int client_i)
{
	std::vector<client>::iterator it;

	std::cout << RED << "Connection Closed\n" << END << std::endl;
	it = web.clients.begin();
	FD_CLR(web.clients[client_i].fd , &web.reads);
	FD_CLR(web.clients[client_i].fd , &web.writes);
	close(web.clients[client_i].fd);

	if (!((web.clients[client_i].response.error
		|| (!web.clients[client_i].map_request.empty()
		&& web.clients[client_i].map_request["Method"] == "GET"))
		&&!std::remove(web.clients[client_i].file_name.c_str())))
		std::cerr << "req not file removed " <<  web.clients[client_i].file_name << std::endl;
	if (web.clients[client_i].response.autoindex
		|| web.clients[client_i].response.generateError)
	{
		if (std::remove(web.clients[client_i].map_request["URI"].c_str()))
			std::cerr << "Failed to remove autoindex file\n";
	}
	
	while (client_i < web.clients.size()
		&& (*it).fd != web.clients[client_i].fd && it != web.clients.end())
		it++;
	if (it != web.clients.end())
		web.clients.erase(it);
	maxFd(web);
}

void	handleConnection(struct webserv& web)
{
	int				i;
	int				j;
	int				size;
	int				k;
	int				flag_fail;
	
	size = web.servers.size();	
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			if (FD_ISSET(web.servers[i].socketFd[j], &web.tmp_read))
			{
					struct client 	newClient;
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
						size = web.servers.size();	
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
						web.clients[k].file_name = "req" + std::to_string(web.req_nbr) + ".txt";
						web.clients[k].body_length = 0;
						web.req_nbr++;
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
			flag_fail = 1;
			receiveRequest(web, web.clients[i], i, flag_fail);
			//size = web.clients.size();	
			if (flag_fail && web.clients[i].request_is_ready == true)
			{
				FD_CLR(web.clients[i].fd , &web.reads);
				if (!web.clients[i].response.error && web.clients[i].map_request["Method"] == "GET")
				{
					std::cout << "Sending get response ...\n";
					get(web, web.clients[i]);
				}
				else if (web.clients[i].response.statusCode < 400 && web.clients[i].map_request["Method"] == "POST")
					post(web, web.clients[i]);
				else if (web.clients[i].response.statusCode < 400 && web.clients[i].map_request["Method"] == "DELETE")
					deleteResponse(web, web.clients[i]);
				std::cout << "\033[00m";

			}
			else if (flag_fail == 0)
			{
				i--;
				size = web.clients.size();
			}
		}
		i++;
	}
	i = 0;
		size = web.clients.size();
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_write) )
		{
			if (web.clients[i].request_is_ready == true)// * && web.clients[i].response_is_ready == true *//*)
			{
				sendResponse(web.clients[i], web, web.clients[i].response.statusCode);
				if (web.clients[i].response.finishReading || web.clients[i].response.error)
				{
					closeConnection(web, i);
				}
			}
		}
		i++;
	}
}


//TODO 