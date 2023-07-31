/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitBody.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 22:54:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/07/29 13:19:56 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"
/*
void	addHeaders(std::string buffer, struct client& clt, int pos)
{
	int find;

	find = buffer.find("\r\n\r\n", pos);
	if (find != -1)
	{
		clt.headers = clt.headers + buffer.substr(pos, find - pos + 2);
	}
}
*/
std::map<std::string, std::string>::iterator content_type_dexist(struct client& clt,std::string pp)
{
	std::map<std::string, std::string>::iterator it = clt.map_request.begin();
	for(;it != clt.map_request.end();it++)
	{
		if(it->first.compare(pp) != 0)
			continue;
		else
			break;
	}
	if(it != clt.map_request.end())
		return (it);
	return (it);
}
void new_string_without_boundary(std::string &buffer, struct client &clt)
{
	
	std::string CTTYPE = "Content-Type";
	//size_t j = 0;
	std::string temp_content_type;
	std::map<std::string,std::string> ContentTypes;
	if(content_type_dexist(clt,CTTYPE) != clt.map_request.end())
	{
		temp_content_type = clt.map_request["Content-Type"];
		temp_content_type = temp_content_type.erase(0,30);
		buffer.erase(0,buffer.find(temp_content_type));
		buffer = buffer.substr(52,buffer.size());
	}
	else
	{
		error(clt,403);
	}
}
void new_string_without_last_boundary(std::string &buffer,struct client &clt)
{
	std::string CTTYPE = "Content-Type";
	//size_t j = 0;
	std::string temp_content_type;
	std::map<std::string,std::string> ContentTypes;
	

	if(content_type_dexist(clt,CTTYPE) != clt.map_request.end())
	{
		temp_content_type = clt.map_request["Content-Type"];
		temp_content_type = temp_content_type.erase(0,30);
		temp_content_type = temp_content_type + "--" ;
		////std::cout << temp_content_type<<std::endl;
		size_t find;
		find = buffer.find(temp_content_type,0);
		if(buffer[find - 1] == '-' && buffer[find - 2] == '-')
			find = find - 2;
		
		////std::cout << buffer;
		// //std::cout<< find<<std::endl;
		if(find != std::string::npos)
			buffer = buffer.erase(find,buffer.size());
	}
	else
	{
		error(clt,403);
	}
}
void new_string_without_last_hexa(std::string &buffer)
{
	std::string fill = "";
	for(size_t i = 0;i < buffer.size();i++)
	{
		fill += buffer[i];
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				break;
			}
		}
	}
	fill += '\n';
	buffer = fill;
}
void new_string_show(std::string &buffer)
{
	size_t	i = 0, j = 0;
	for(;i < buffer.size();i++)
	{
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				////std::cout << buffer<<std::endl;
				buffer.erase(0,i + 1);
				////std::cout << buffer<<std::endl;
				j+= 1;
				if(j > 0)
					break;
			}
		}

	}
}
void new_string_without_first_hexa(std::string &buffer)
{
	size_t	i = 0;
	std::string fill = "";
	for(;i < buffer.size();i++)
	{
		fill += buffer[i];
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				buffer[i] = '\n';
				break;
			}
		}
	}
	int find = buffer.find(fill.c_str(),0);
	(void)find;
	buffer = buffer.substr(fill.size(),buffer.size());
}
void new_string_without_first_and_last_v2(std::string &buffer)
{
	size_t	i = 0;
	for(;i < buffer.size();i++)
	{
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				if(buffer[i + 2] == '\r')
				{
					if(buffer[i + 3] == '\n')
					{
						buffer.erase(0,i + 3);
					}
				}
			}
		}

	}
}
void new_string_without_first_and_last(std::string &buffer)
{
	size_t	i = 0;
	for(;i < buffer.size();i++)
	{
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				if(buffer[i + 2] == '\r')
				{
					if(buffer[i + 3] == '\n')
					{
						buffer[i + 3] = ' ';
					}
				}
			}
		}

	}
}
std::string extractFieldName(const std::string& header) {
    std::string name;
    size_t nameStart = header.find("name=\"");
    if (nameStart != std::string::npos) {
        nameStart += 6; // Move to the first character after "name=\""
        size_t nameEnd = header.find("\"", nameStart);
        if (nameEnd != std::string::npos) {
            name = header.substr(nameStart, nameEnd - nameStart);
        }
    }
    return name;
}
std::string extractFieldFileName(const std::string& header) {
    std::string name;
    size_t nameStart = header.find("ilename=\"");
    if (nameStart != std::string::npos) {
        nameStart += 9; // Move to the first character after "name=\""
        size_t nameEnd = header.find("\"", nameStart);
        if (nameEnd != std::string::npos) {
            name = header.substr(nameStart, nameEnd - nameStart);
        }
    }
    return name;
}
void new_string_erase_content_disposition(std::string &buffer)
{
	//size_t	i = 0;
	//int found_n = 0;
	if(buffer.find("filename") == std::string::npos)
		buffer = buffer.erase(0,34);
	else
		buffer = buffer.erase(0,32);
	std::string name = extractFieldName(buffer);
	size_t i = 6 + name.size() + 1;
	// //std::cout << buffer<<std::endl;
	buffer = buffer.erase(0,i);
	// //std::cout << buffer<<std::endl;
	if(buffer.find("ilename",0) != std::string::npos)
	{
		std::string ilename = extractFieldFileName(buffer);
		i = 10 + ilename.size() + 3;
		buffer.erase(0, i);
		// //std::cout << buffer<<std::endl;

	}
}
void new_string_erase_content_type(std::string &buffer)
{
	size_t find = buffer.find("Content-Type");
	if(find != std::string::npos)
	{
		size_t find_n = buffer.find("\n",find);
		buffer.erase(find,find_n - find + 1);
	}
}
void new_string_buff_1(std::string &buffer)
{
	size_t	i = 0;
	for(;i < buffer.size();i++)
	{
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				buffer[i] = 'f';
				buffer[i + 1] = 'o';
				return;
			}
		}

	}
}
void	splitBody(std::string buffer, struct client& clt, int n_byte_readed)
{
	int 					size;
	int 					find;
	// size_t						find_filename;

	find = -1;
	size = buffer.size();

	std::string buffer2 = buffer;
	if (size == 0)
		return ;
	// //std::cout << buffer <<"   "<<std::endl;
	if (clt.nbr_of_reads == 1 && clt.headers.size() == 0)
	{
		find = buffer.find("\r\n\r\n");
		if (find > 0)
		{
			clt.headers = buffer.substr(0, find + 2);
		}
	}
	if (clt.post_flag)
	{
		if (clt.bodys.get_body_type == 0)
			getBodyType(buffer, clt.bodys);
		////std::cout << "the buffer is ="<< buffer<<std::endl;
		// //std::cout << "###############\n";
		multiTypes(buffer,buffer2,clt,n_byte_readed);
	}
	// find_filename = buffer2.find("filename",0);
	// // //std::cout << clt.bodys.chunks_flag <<std::endl;
	// // //std::cout << clt.bodys.boundary_flag <<std::endl;
	// // //std::cout << clt.bodys.content_length_flag <<std::endl;
	// if((clt.bodys.chunks_flag == 1 && clt.bodys.boundary_flag == 0 && clt.bodys.content_length_flag == 0 && clt.bodys.binary_flag == 0) || (clt.bodys.chunks_flag == 1 && clt.bodys.boundary_flag == 0 && clt.bodys.content_length_flag == 1 && clt.bodys.binary_flag == 0))
	// {				
	// 	new_string_without_first_and_last(buffer2);
	// 	new_string_buff_1(buffer2);
	// 	new_string_without_first_hexa(buffer2);
	// 	new_string_without_last_hexa(buffer2);
	// }
	// else if((clt.bodys.chunks_flag == 1 && clt.bodys.boundary_flag == 1 && clt.bodys.content_length_flag == 1) || ((clt.bodys.chunks_flag == 1 && clt.bodys.boundary_flag == 1 && clt.bodys.content_length_flag == 0)))
	// {		
	// 		// new_string_without_boundary(buffer2,clt);
	// 		// new_string_without_last_boundary(buffer2,clt);
	// 		// new_string_erase_content_type(buffer2);
	// 		// new_string_erase_content_disposition(buffer2);
	// 		//  new_string_without_first_and_last(buffer2);
	// 		// new_string_buff_1(buffer2);
	// 		// new_string_without_first_hexa(buffer2);
	// 		// if(find_filename != std::string::npos)
	// 		// 	new_string_show(buffer2);
	// 		// else
	// 		// 	new_string_without_last_hexa(buffer2);
	// }
	// else if((clt.bodys.chunks_flag == 0 && clt.bodys.boundary_flag == 1 && clt.bodys.content_length_flag == 0 ) || ( clt.bodys.chunks_flag == 0 && clt.bodys.boundary_flag == 1 && clt.bodys.content_length_flag == 1 ))
	// {	
	// 		new_string_without_boundary(buffer2,clt);
	// 		new_string_without_last_boundary(buffer2,clt);
	// 		new_string_erase_content_type(buffer2);
	// 		new_string_erase_content_disposition(buffer2);
	// 		new_string_without_first_and_last_v2(buffer2);
	// 		//new_string_show(buffer2);
	// }
	// else if((clt.bodys.chunks_flag == 1 && clt.bodys.boundary_flag == 0 && clt.bodys.content_length_flag == 0 && clt.bodys.binary_flag == 1) || (clt.bodys.chunks_flag == 1 && clt.bodys.boundary_flag == 0 && clt.bodys.content_length_flag == 1 && clt.bodys.binary_flag == 1))
	// {
	// 	new_string_without_first_and_last(buffer2);
	// 	new_string_buff_1(buffer2);
	// 	new_string_without_first_hexa(buffer2);
	// 	new_string_without_last_hexa(buffer2);
	// }
	// //std::cout << buffer2;
	// std::ofstream out;
	// out.open(clt.upload_files[0].filename,std::ios::app | std::ios::out);
	// if(!out.is_open())
	// {
	// 	std::cerr << "Error: input file\n";
	// }
	// out << buffer2;
	// clt.upload_files[0].len = buffer2.size();
	// out.close();
	// //std::cout << "go" << std::endl;
}
