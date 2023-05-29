/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitBody.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 22:54:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/29 12:55:02 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


void	findStartAndEnd(int& start, int& end, std::string buffer, int pos, struct body bodys)
{
	std::string		boundary;
	int				find;

	if (bodys.boundary_flag)
	{
		boundary = bodys.boundary.substr(0, bodys.boundary.size() - 2);
		find = buffer.find(boundary, pos);
		if (find > 0)
		{
			end = find - 2;
		}
		return ;
	}	
	find = buffer.find("\r\n", pos);
	if (find != -1)
	{
		end = find;
		return ;
	}
	end = -1;

}

void	addHeaders(std::string buffer, struct client& clt, int pos)
{
	int find;

	find = buffer.find("\r\n\r\n", pos);
	if (find != -1)
	{
		clt.headers = clt.headers + buffer.substr(pos, find - pos + 2);
	}
	//std::cout << "New headers : [" << clt.headers << "]" << std::endl;
}

void	splitBody(std::string buffer, struct client& clt)
{
	int 					i;
	int						pos;
	int 					j;
	int 					size;
	int 					find;
	int						start;//start of the file in the current chunk
	int						end;// end of the file in the current chunk
	struct	uploadFiles		upload_files;

	std::cout << "Buffer : [" << buffer.size() << "]" << std::endl;
	i = 0;
	find = -1;
	size = buffer.size();
	if (size == 0)
		return ;
	if (clt.nbr_of_reads == 1 && clt.headers.size() == 0)
	{
		find = buffer.find("\r\n\r\n");
		if (find > 0)
		{
			std::cout << "Find it \n";
			clt.headers = buffer.substr(0, find + 2);
			std::cout << "Headers : {" << clt.headers << "}" << std::endl;
		}
	}
	//std::cout << "Post flag : " << clt.post_flag << std::endl;
	if (clt.post_flag)
	{
		pos = 0;
		find = buffer.find("Content-Disposition:", pos);
		if (find >= 0)
		{
			while (find >= 0)
			{
				//std::cout << "Pos 1 : " << pos << std::endl;
				pos = find;
				addHeaders(buffer, clt, pos);
				find = buffer.find("filename=" , pos);
				if (find > 0)
				{
					i = find + 10;
					j = 0;
					while (i + j < size && buffer[i + j] != '\"' && buffer[i + j] != '\r')
						j++;
					if (j && i < size)
					{
						upload_files.filename = buffer.substr(i, j);
						std::cout << "Find file : " << upload_files.filename << std::endl;
						start = buffer.find("\r\n\r\n", pos) + 4;
						findStartAndEnd(start, end, buffer, pos, clt.bodys);
						if (end > 0)
						{
							upload_files.file->open(upload_files.filename.c_str(), std::fstream::app | std::fstream::out);
							upload_files.file->write(buffer.c_str(), buffer.size());
							upload_files.file->close();
							clt.upload_files.push_back(upload_files);
							;//std::cout << "Chunk [[" << buffer.substr(start, end - start) << "]]" << std::endl; 
						}
					}
				}
				else
					break;
				while (pos < size && buffer[pos] != '\n')
					pos++;
				//std::cout << "Pos 2 : " << pos << std::endl;
				find = buffer.find("Content-Disposition:", pos);
				//std::cout << "Find : -> " << find << std::endl;
			}
		}
		else
		{
			start = 0;
			end = buffer.find("\r\n");
			int index = clt.upload_files.size();
			int size = buffer.size();
			std::string temp;
			if (end > 0 && end < size)
			{
				if (index)
				{
					clt.upload_files[index - 1].file->open(clt.upload_files[index - 1].filename,  std::fstream::app | std::fstream::out);
					temp = buffer.substr(start, end);
					clt.upload_files[index - 1].file->write(temp.c_str(), temp.size());
					clt.upload_files[index - 1].file->close();
				}
				else
				{
					//std::cout << "here " << std::endl;
					start = buffer.find("\r\n\r\n", 0);

					//std::cout << "here " << std::endl;
					if (start > 0 && start < size)
					{
					//std::cout << "here " << std::endl;
					//		std::cout << "st : " << start << "en : " << end << "sz : " << size << std::endl; 
						end = buffer.find("\r\n\r\n", start);
					//	std::cout << "here 1" << std::endl;
						if (end > 0 && (end - (start + 4) > size))	
						{
					//		std::cout << "st : " << start << "en : " << end << "sz : " << size << "cl " << (end - (start + 4)) << std::endl; 
							clt.body_data = clt.body_data + temp.substr(start + 4, end - (start + 4));
						}
						else if (clt.bodys.boundary_flag)
						{
					//std::cout << "here " << std::endl;
							end = buffer.find(clt.bodys.boundary, start + 4);
							if (end > 0 && end - size)
								clt.body_data = clt.body_data + temp.substr(start + 4, end - start);
					//std::cout << "here " << std::endl;
						}
						else if (size - (start + 4) > 0)
						{
					//		std::cout << "here op " << size << " " << (start + 4) << " " << (size - (start + 4)) << std::endl;
							clt.body_data = clt.body_data + buffer.substr(start + 4, size - (start + 4));
						}
					}
				}
			}
			std::cout << "Body data : [" << clt.body_data << "]" << std::endl;
		}
	}
}
