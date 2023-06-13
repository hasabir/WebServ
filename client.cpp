/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 00:19:32 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/12 14:44:19 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


uploadFiles::uploadFiles()
{
	//std::cout << "Upload constructor called" << std::endl;
	len = 0;
	filename = "";
	file = new std::fstream();
}

uploadFiles::~uploadFiles()
{
	//std::cout << "Upload destructor called" << std::endl;
	delete file;
}

uploadFiles::uploadFiles(const uploadFiles& rhs)
{
	len = rhs.len;
	filename = rhs.filename;
	file = new std::fstream();
}

uploadFiles&	uploadFiles::operator=(const uploadFiles& rhs)
{
	if (this != &rhs)
	{
		len = rhs.len;
		filename = rhs.filename;
		file = new std::fstream();
	}
	return (*this);
}

client::client()
{	
	config = -1;
	location = -1;
	len = 0;
	fd = -1;
	request_is_ready = false;
	response_is_ready = false;
	bodys.get_body_type = 0;
	bodys.rd_bytes = 0;
	bodys.chunks_flag = 0;
	bodys.boundary_flag = 0;
	bodys.content_length_flag = 0;
	bodys.cr_nl_flag = 0;
	bodys.n_chunks = 0;
	bodys.cr_index = -1;
	bodys.chunks_len = 0;
	bodys.chunks_con_len = 0;
	bodys.content_len = 0;
	bodys.content_disposition = 0;
	bodys.boundary.assign("");
	headers.assign("");
	file = new std::fstream();
	file_name.assign("");
	body_data.assign("");
	nbr_of_reads = 0;
	post_flag = 0;
	body_data.assign("");
	body_length = 0;
}

client::~client() 
{
	delete file;
}

client::client(const client& rhs)
{
	int				i;
	int				size;

	// map_request = rhs.map_request;
	// map_response = rhs.map_response;
	config = rhs.config;
	location = rhs.location;
	request = rhs.request;
	response = rhs.response;
	addr = rhs.addr;
	len = rhs.len;
	fd = rhs.fd;
	request_is_ready = rhs.request_is_ready;
	response_is_ready = rhs.response_is_ready;
	nbr_of_reads = rhs.nbr_of_reads;
	post_flag = rhs.post_flag;
	body_data = rhs.body_data;
	bodys.get_body_type = rhs.bodys.get_body_type;
	bodys.rd_bytes = rhs.bodys.rd_bytes;
	bodys.chunks_flag = rhs.bodys.chunks_flag;
	bodys.boundary_flag = rhs.bodys.boundary_flag;
	bodys.content_length_flag = rhs.bodys.content_length_flag;
	bodys.cr_nl_flag = rhs.bodys.cr_nl_flag;
	bodys.n_chunks = rhs.bodys.n_chunks;
	bodys.cr_index = rhs.bodys.cr_index;
	bodys.chunks_len = rhs.bodys.chunks_len;
	bodys.chunks_con_len = rhs.bodys.chunks_con_len;
	bodys.content_len = rhs.bodys.content_len;
	bodys.content_disposition = rhs.bodys.content_disposition;
	bodys.boundary = rhs.bodys.boundary;
	headers = rhs.headers;
	file = new std::fstream();
	file_name = rhs.file_name;
	body_data = rhs.body_data;
	nbr_of_reads = rhs.nbr_of_reads;
	post_flag = rhs.post_flag;
	body_length = rhs.body_length;
	i = 0;
	size = rhs.upload_files.size();
	while (i < size)
	{
		upload_files.push_back(rhs.upload_files[i]);
		i++;
	}
}

client&		client::operator=(const client& rhs)
{
	int i;
	int size;

	if (this != &rhs)
	{
		// map_request = rhs.map_request;
		// map_response = rhs.map_response;
		config = rhs.config;
		location = rhs.location;
		request = rhs.request;
		response = rhs.response;
		addr = rhs.addr;
		len = rhs.len;
		fd = rhs.fd;
		request_is_ready = rhs.request_is_ready;
		response_is_ready = rhs.response_is_ready;
		nbr_of_reads = rhs.nbr_of_reads;
		post_flag = rhs.post_flag;
		body_data = rhs.body_data;
		bodys.get_body_type = rhs.bodys.get_body_type;
		bodys.rd_bytes = rhs.bodys.rd_bytes;
		bodys.chunks_flag = rhs.bodys.chunks_flag;
		bodys.boundary_flag = rhs.bodys.boundary_flag;
		bodys.content_length_flag = rhs.bodys.content_length_flag;
		bodys.cr_nl_flag = rhs.bodys.cr_nl_flag;
		bodys.n_chunks = rhs.bodys.n_chunks;
		bodys.cr_index = rhs.bodys.cr_index;
		bodys.chunks_len = rhs.bodys.chunks_len;
		bodys.chunks_con_len = rhs.bodys.chunks_con_len;
		bodys.content_len = rhs.bodys.content_len;
		bodys.content_disposition = rhs.bodys.content_disposition;
		bodys.boundary = rhs.bodys.boundary;
		headers = rhs.headers;
		file = new std::fstream();
		file_name = rhs.file_name;
		body_data = rhs.body_data;
		nbr_of_reads = rhs.nbr_of_reads;
		post_flag = rhs.post_flag;
		body_length = rhs.body_length;
		i = 0;
		size = rhs.upload_files.size();
		while (i < size)
		{
			upload_files.push_back(rhs.upload_files[i]);
			i++;
		}
	}
	return (*this);
}
