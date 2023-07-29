/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:04:07 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/07/27 19:38:52 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	closeConnection(struct webserv& web, int client_i)
{
	std::vector<client>::iterator it;

	// std::cout << RED << "\nConnection Closed " << web.clients[client_i].fd << END << std::endl;
	it = web.clients.begin();
	FD_CLR(web.clients[client_i].fd , &web.reads);
	FD_CLR(web.clients[client_i].fd , &web.writes);
	close(web.clients[client_i].fd);
	
	//
	// if (!web.clients[client_i].map_request.empty())
	// 	std::remove(web.clients[client_i].file_name.c_str());
	
	// if (web.clients[client_i].response.autoindex
	// 	|| web.clients[client_i].response.generateError
	// 	|| web.clients[client_i].response.cgi)
	// {
		if (web.clients[client_i].response.remove)
			std::remove(web.clients[client_i].map_request["URI"].c_str());
		// std::cerr << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
	// }
	// if (web.clients[client_i].map_request["Method"] == "POST"
	// 	&& web.clients[client_i].response.cgi)
	// 	std::remove(web.clients[client_i].upload_files[0].filename.c_str());
	while ((unsigned int)client_i < web.clients.size()
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
		while ((unsigned long)j < web.servers[i].socketFd.size())
		{
			if (FD_ISSET(web.servers[i].socketFd[j], &web.tmp_read))
			{
					struct client 	newClient;
					newClient.len = sizeof(newClient.addr);
					newClient.fd = accept(web.servers[i].socketFd[j],
						(struct sockaddr *)&newClient.addr, &newClient.len);
					
					if (newClient.fd < 0)
					{
						std::cerr << "Error : Fail connecting to client" << std::endl;
						return ;
					}
					else
					{
						int flags = fcntl(newClient.fd, F_GETFL, 0);//!
						flags |= O_NONBLOCK;//!
						fcntl(newClient.fd, F_SETFL, flags);//!
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
						web.clients[k].file_name = "Requests/.req" + intToString(web.req_nbr) + ".txt";
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
	i = 0;
	while ((unsigned long)i < web.clients.size())
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_read))
		{
			
			flag_fail = 1;
			// try{
				receiveRequest(web, web.clients[i], i, flag_fail);
			// }
			// catch(std::exception &e){error(web.clients[i], 500);}
			if (flag_fail && web.clients[i].request_is_ready == true && !web.clients[i].response.error)
			{
				FD_CLR(web.clients[i].fd , &web.reads);
				if (web.clients[i].map_request["Method"] == "GET")
					get(web, web.clients[i]);
				else if (web.clients[i].map_request["Method"] == "POST")
					post(web, web.clients[i]);
				else if (web.clients[i].map_request["Method"] == "DELETE")
					deleteResponse(web, web.clients[i]);

			}
		}
		i++;
	}
	i = 0;
	while ((unsigned long)i < web.clients.size())
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_write) )
		{
			//TODO rm req files
			// std::cout << GREEN << "#############\n" << END;
			if (web.clients[i].request_is_ready == true)// * && web.clients[i].response_is_ready == true *//*)
			{
				if (!web.clients[i].map_request.empty())
					std::remove(web.clients[i].file_name.c_str());
				if (web.clients[i].cgi.loop_detected)
				{
					if (get_time(web.clients[i]) == 35)
					{
						generate_CGI_file(web.clients[i], web.clients[i].map_request["URI"]);
						web.clients[i].cgi.loop_detected = false;
					}
				}
				if (web.clients[i].cgi.loop_detected == false)
					sendResponse(web.clients[i], web, web.clients[i].response.statusCode);
				if (web.clients[i].response.finishReading || web.clients[i].response.error)
					closeConnection(web, i);
			}
		}
		i++;
	}
}

