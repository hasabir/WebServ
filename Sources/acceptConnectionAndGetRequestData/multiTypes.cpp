/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiTypes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 19:54:34 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/29 12:50:30 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	getFileSize(struct uploadFiles& file)
{
	file.file->open(file.filename.c_str(), std::fstream::in | std::fstream::binary );

	if (file.file->is_open() == true)
	{
		file.file->seekg(0, std::fstream::end);  // Seek to the end of the file
        //std::streampos fileSize = file.file->tellg();  // Get the current position
		file.file->close();
	}
}


int		isADerective(std::string buffer, int find, int size)
{
	int find_name;
	int find_cr_nl;
	int find_filename;
	(void)size;
	find_cr_nl = buffer.find("\r\n", find);
	find_name = buffer.find("name=", find);
	find_filename = buffer.find("filename=", find);
	if (find_cr_nl != -1 && find_name != -1 && find_filename != -1 && find_name < find_cr_nl && find_filename < find_cr_nl)
		return (1);
	if (find_cr_nl != -1 && find_name != -1  && find_name < find_cr_nl)
		return (1);
	return (0);

}

void	getFilename(std::string buffer, int file_index, struct uploadFiles& upload_files, int find, int fd,struct client &clt)
{
	int 			find_filename;
	int 			i;
	int				size;
	std::string		temp;
	int 			find_cr;
	(void)fd;
	find_filename = -1;
	size = buffer.size();
	find_cr = buffer.find("\r\n", find);
	temp = clt.temp_header;
	// std::cout << clt.temp_header<<std::endl;
	if (size && find_cr != -1)
	{

		temp = buffer.substr(find, find_cr - find);
		find_filename = temp.find("filename=");
		if (find_filename != -1)
			find_filename = buffer.find("filename=", find);
	}
	else if (size)
		find_filename = buffer.find("filename=", find);
	if (find != -1 && find_filename != -1)
	{
		find_filename += 10;
		i = 0;
		while (find_filename + i < size && buffer[find_filename + i] != '\"')
			i++;
		if (i)
		{
			temp = buffer.substr(find_filename, i);
			find_filename = temp.find_last_of("/");
			if (find_filename != -1)
			{
				temp = temp.substr(find_filename + 1);
				if (temp.size() == 0)
					temp =  "text_" + std::to_string(file_index) + ".txt";
			}
		}
		else
			temp = "text_" + std::to_string(file_index) + ".txt";
		upload_files.filename = "./www/uploads/" + temp;
		upload_files.just_the_file = temp;
		upload_files.no_name = "no";
	}
	else if(clt.temp_header[0] != 0) {
		temp = clt.temp_header;
		size_t content_di = temp.find("Content-Disposition");
		(void)content_di;
		find_filename = temp.find("filename=");
		find_filename += 10;
		int j = 0;
		while ((size_t)find_filename + j < temp.size() && temp[find_filename + j] != '\"')
			j++;
		temp = temp.substr(find_filename,j);
		upload_files.filename = "./www/uploads/" + temp;
		upload_files.just_the_file = temp;
		upload_files.no_name = "no";

	}
	else 
	{
		std::cout << "Hereeee"<<std::endl;
		temp = "text_" + std::to_string(file_index) + ".txt";
		upload_files.filename = "./www/uploads/" + temp;
		upload_files.just_the_file = temp;
		upload_files.no_name = "yes";

	}
}


void	multiTypes(std::string buffer, struct client& clt)
{
	int						find_cr;
	int						find;
	int						size;
	int						i;
	int						flag_postman_curl;
	std::string				hex;
	std::string				temp;
	struct  uploadFiles     upload_files;
	std::string				theEnd;

	theEnd = "\r\n\r\n" + clt.bodys.boundary;
	flag_postman_curl = 1;
	size = buffer.size();
	i = 0;
	if (clt.bodys.content_length_flag == 1 && clt.bodys.chunks_flag == 0 && clt.bodys.boundary_flag == 0)
	{
		find = 0;
		if (clt.nbr_of_reads == 1)
		{
			// std::cout << BLUE<<clt.map_request["Content-Disposition"]<<std::endl<<END;
			find_cr = buffer.find("\r\n\r\n");
			if(find_cr != -1)
				clt.temp_header =  buffer.substr(0,find_cr + 4);
			// std::cout << clt.temp_header<< std::endl;
			if (find_cr != -1)
			{
				buffer = buffer.substr(find_cr + 4);
			}
		}
		i = clt.upload_files.size();
		if (buffer.size() && i == 0)
		{
			//std::cout << clt.nbr_of_reads<<std::endl;
			size_t founder_name = clt.temp_header.find("Content-Disposition");
			if(founder_name != std::string::npos)
			{	
				size_t find_r_n = clt.temp_header.find("\n",founder_name);
				clt.temp_header = clt.temp_header.substr(founder_name,find_r_n - founder_name);
			}
			else
			{
				clt.temp_header = "";
			}
			getFilename(buffer, clt.upload_files.size(), upload_files, -1, clt.fd,clt);
			upload_files.file->open(upload_files.filename.c_str(),std::fstream::app | std::fstream::out);
			//sleep(2);
			if (upload_files.file->is_open() == true)
			{
				upload_files.file->close();
				clt.upload_files.push_back(upload_files);
			}
		}
		i = clt.upload_files.size();
		if (i)
		{
			temp.assign("");
			temp = clt.upload_files[i - 1].filename;
			clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::app | std::fstream::out);
			if (buffer.size() >= clt.bodys.content_len)
				clt.upload_files[i - 1].file->write(buffer.c_str(), clt.bodys.content_len);
			else
			{
				clt.upload_files[i - 1].file->write(buffer.c_str(), buffer.size());
			}
			clt.upload_files[i - 1].file->close();
		}
	}
	else if (clt.bodys.boundary_flag == 1 || clt.bodys.chunks_flag == 1)
	{
		int 			flag;
		std::string		bndry;
		std::string		tmp;

		flag = 1;
		bndry = clt.bodys.boundary.substr(0, clt.bodys.boundary.size() - 2);
		while (buffer.size())
		{
			find = 0;
			if (clt.nbr_of_reads == 1 && flag)
			{
				find_cr = buffer.find("\r\n\r\n");
				find = buffer.find(bndry, find_cr);
				if (find != -1)
				{
					buffer = buffer.substr(find);
				}
				flag = 0;
			}
			else
			{
				find = buffer.find(bndry);
				if ((find >= 0 && find <= 4) ||  (clt.bodys.content_disposition == 1 && buffer.size()))
				{
					find = buffer.find("Content-Disposition:");
					if (find != -1 && isADerective(buffer, find, size) == 1 && clt.bodys.content_disposition == 0)
					{
						getFilename(buffer, clt.upload_files.size(), upload_files, find, clt.fd,clt);
						upload_files.file->open(upload_files.filename.c_str(), std::fstream::app | std::fstream::out);
						if (upload_files.file->is_open() == true)
						{
							upload_files.file->close();
							clt.upload_files.push_back(upload_files);
						}
						find_cr = buffer.find("\r\n\r\n", find);
						buffer = buffer.substr(find_cr + 4);
						clt.bodys.content_disposition = 1;
					}
					else if (clt.bodys.content_disposition == 0)
					{
						if (clt.upload_files.size() == 0)
						{
							getFilename(buffer, clt.upload_files.size(), upload_files, find, clt.fd,clt);
							upload_files.file->open(upload_files.filename.c_str(), std::fstream::app | std::fstream::out);
							if (upload_files.file->is_open() == true)
							{
								upload_files.file->close();
								clt.upload_files.push_back(upload_files);
							}
							clt.bodys.content_disposition = 1;
						}
						else 
							clt.bodys.content_disposition = 1;
					}
					if (clt.bodys.content_disposition == 1)
					{
						find = buffer.find(bndry);
						if (find == -1) // check for npos
						{
							find = buffer.find(clt.bodys.boundary);
						}
						i = clt.upload_files.size();
						if (find != -1 && i)
						{
							if (find > 1)
								find = find - 2;
							temp.assign("");
							temp = clt.upload_files[i - 1].filename;
							clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::app | std::fstream::out);
							temp = buffer.substr(0, find);
							clt.upload_files[i - 1].file->write(temp.c_str(), temp.size());
							clt.upload_files[i - 1].file->close();
								buffer = buffer.substr(find + 2);
							clt.bodys.content_disposition = 0;
						}
						else if (buffer.size() && i)
						{
							temp.assign("");
							temp = clt.upload_files[i - 1].filename;
							clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::app | std::fstream::out);
							temp = buffer.substr(0, buffer.size());
							clt.upload_files[i - 1].file->write(temp.c_str(), temp.size());
							clt.upload_files[i - 1].file->close();
							buffer = buffer.assign("");
							clt.bodys.content_disposition = 1;
						}
					}
					i = buffer.find(clt.bodys.boundary);
					if (i >= 0 && i < 3)
					{
						buffer.assign("");
					}
				}
			}
		}
	}
}
