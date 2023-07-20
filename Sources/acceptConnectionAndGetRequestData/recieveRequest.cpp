/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 11:40:12 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/07/20 11:43:14 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

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

void	receiveRequest(struct webserv& web, struct client& clt, int clt_i, int& flag_fail)
{
	char							line[100000];
	int 							i;
	int			n_byte_readed;
	std::string	buff;

	memset(line, 0, 100000);
	n_byte_readed = 0;
	// n_byte_readed = recv(clt.fd, line, 99999, 0);
	if ((n_byte_readed = recv(clt.fd, line, 99999, 0)) < 0)
	{
		throw std::runtime_error("----------------------- RECSEIV ");
	}
	line[n_byte_readed] = 0;
	buff.assign("");
	if (n_byte_readed < 0)
	{
		std::cout << PURPLE << "--------------- 1  receiveRequest  \n" << END;
		closeConnection(web, clt_i);	
		flag_fail = 0;
		return ;
	}
	if (n_byte_readed == 0)
	{
		if (clt.nbr_of_reads == 0)
		{
			std::cout << PURPLE << "--------------- 2 receiveRequest  \n" << END;
			FD_CLR(web.clients[clt_i].fd , &web.reads);
			closeConnection(web, clt_i);
			flag_fail = 0;
		}
		return ;
	}
	else
	{
		if (clt.nbr_of_reads == 0)
			clt.file->open(clt.file_name.c_str(),  std::fstream::out);
		else
			clt.file->open(clt.file_name.c_str(),  std::fstream::app | std::fstream::out);
		if (!clt.file->is_open())
		{
			std::cerr << "Can not open file." << std::endl;
			std::cout << PURPLE << "--------------- 3  receiveRequest \n" << END;
			closeConnection(web, clt_i);
			flag_fail = 0;
			return;
		}
		//you need to check for file is opned and doing what the sutiation needs
		clt.file->write((char *) line, n_byte_readed); //	clt.buffer << line;
		clt.file->close();
		clt.bodys.rd_bytes = clt.bodys.rd_bytes + n_byte_readed;
		buff.assign(line, n_byte_readed);
		if (clt.nbr_of_reads == 0)
		{
			if (buff.find("POST") == 0)
			{
				clt.post_flag = 1;
			}
		}
		clt.nbr_of_reads++;
	}

	/*****************************************************************/
	if (clt.nbr_of_reads == 1)
	{
		parseRequest(web, web.clients[clt_i]);
		if (web.clients[clt_i].response.error)
		{
			web.clients[clt_i].response.error = true;
			FD_CLR(web.clients[clt_i].fd , &web.reads);
			int n_byte_readed = 0;
			char line[2];
			n_byte_readed = recv(web.clients[clt_i].fd, line, 0, MSG_PEEK);
			if (n_byte_readed < 0)
			{
				// std::cout << PURPLE << "--------------- 4 receiveRequest  \n" << END;
				closeConnection(web, clt_i);
				flag_fail = 0;
				return ;
			}
			// std::cout << PURPLE <<"!!!!!!!!!!!!!!! execute cgi = |" << clt.cgi.extention << "|\n" << END;
			sendResponse(web.clients[clt_i], web, web.clients[clt_i].response.statusCode);

			// std::cout << PURPLE << "--------------- 5 receiveRequest  \n" << END;
			closeConnection(web, clt_i);
			flag_fail = 0;
			return ;
		}
	}
	/*****************************************************************/

	web.clients[clt_i].response.error = false;
	if (clt.post_flag == 1)
	{
		splitBody(buff, clt);
	}
	else
	{
		i = buff.find("\r\n\r\n");
		if (i != -1)
		{
			clt.request_is_ready = true;
			FD_SET(clt.fd, &web.writes);
			return;
		}

	}
	if (endOfTheRequest(buff, clt.bodys) == 0)
	{
		clt.request_is_ready = true;
		if (clt.post_flag)
			getFilesLength(clt);
		FD_SET(clt.fd, &web.writes);
	}
}
