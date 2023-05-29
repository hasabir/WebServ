/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 00:19:32 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/28 16:25:55 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

client::client()
{	
	request_is_ready = false;
	response_is_ready = false;
	fd = -1;
	len = 0;
	bodys.get_body_type = 0;
	bodys.chunks_flag = 0;
	bodys.boundary_flag = 0;
	bodys.content_length_flag = 0;
	bodys.cr_nl_flag = 0;
	bodys.n_chunks = 0;
	bodys.cr_index = 0;
	bodys.chunks_len = 0;
	bodys.chunks_con_len = 0;
	bodys.content_len = 0;
	bodys.boundary.assign("");
	nbr_of_reads = 0;
	post_flag = 0;
	body_data.assign("");
	file = new std::fstream();
}

client::~client() 
{
	int i;
	int size;

	i = 0;
	size = upload_files.size();
	while (i < size)
	{
		delete (upload_files[i].file);
		i++;
	}
	delete file;
}

client::client(const client& rhs)
{
	int				i;
	int				size;

	request = rhs.request;
	response = rhs.response;
	addr = rhs.addr;
	len = rhs.len;
	fd = rhs.fd;
	request_is_ready = rhs.request_is_ready;
	response_is_ready = rhs.response_is_ready;
	bodys = rhs.bodys;
	headers = rhs.headers;
	nbr_of_reads = rhs.nbr_of_reads;
	post_flag = rhs.post_flag;
	body_data = rhs.body_data;
	i = 0;
	size = rhs.upload_files.size();
	while (i < size)
	{
		upload_files[i].filename = rhs.upload_files[i].filename;
		upload_files[i].file = new std::fstream();
		i++;
	}
	file = new std::fstream();
	
}

client&		client::operator=(const client& rhs)
{
	int i;
	int size;

	if (this != &rhs)
	{
		request = rhs.request;
		response = rhs.response;
		addr = rhs.addr;
		len = rhs.len;
		fd = rhs.fd;
		request_is_ready = rhs.request_is_ready;
		response_is_ready = rhs.response_is_ready;
		bodys = rhs.bodys;
		headers = rhs.headers;	
		nbr_of_reads = rhs.nbr_of_reads;
		post_flag = rhs.post_flag;
		body_data = rhs.body_data;
		i = 0;
		size = rhs.upload_files.size();
		while (i < size)
		{
			upload_files[i].filename = rhs.upload_files[i].filename;
			upload_files[i].file = new std::fstream();
			i++;
		}
		file = new std::fstream();
	}
	return (*this);
}
