/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiTypes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 19:54:34 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/07/29 18:22:11 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

int        isHex(char c)
{
    int i = 0;
    std::string hexs = "0123456789abcdef";
    while (i < 16)
    {
        if (c == hexs[i])
            return (1);
        i++;
    }
    return (0);
}

size_t FindStringInBuffer(const char *buffer, size_t buffer_size, const char *str, int pos)
{
    size_t str_len = strlen(str);
    if (buffer_size < str_len)
        return std::string::npos;
    if (pos == 0)
    {
        for (size_t i = 0; i < buffer_size - str_len + 1; i++)
            if (strncmp(buffer + i, str, str_len) == 0)
                return i;
    }
    else
    {
        for (size_t i = pos; i < buffer_size - str_len + 1; i++)
            if (strncmp(buffer + i, str, str_len) == 0)
                return i;
    }
    return std::string::npos;
}

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
std::map<std::string, std::string>::iterator content_type_dexist2(struct client& clt,std::string pp)
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
void new_string_without_boundary2(std::string &buffer, struct client &clt)
{
	
	std::string CTTYPE = "Content-Type";
	//size_t j = 0;
	std::string temp_content_type;
	std::map<std::string,std::string> ContentTypes;
	if(content_type_dexist2(clt,CTTYPE) != clt.map_request.end())
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
void new_string_without_last_boundary2(std::string &buffer,struct client &clt)
{
	std::string CTTYPE = "Content-Type";
	//size_t j = 0;
	std::string temp_content_type;
	std::map<std::string,std::string> ContentTypes;
	

	if(content_type_dexist2(clt,CTTYPE) != clt.map_request.end())
	{
		temp_content_type = clt.map_request["Content-Type"];
		temp_content_type = temp_content_type.erase(0,30);
		temp_content_type = temp_content_type + "--" ;
		////////std::cout << temp_content_type<<std::endl;
		size_t find;
		find = buffer.find(temp_content_type,0);
		if(buffer[find - 1] == '-' && buffer[find - 2] == '-')
			find = find - 2;
		
		////////std::cout << buffer;
		// //std::cout<< find<<std::endl;
		if(find != std::string::npos)
			buffer = buffer.erase(find,buffer.size());
	}
	else
	{
		error(clt,403);
	}
}
void new_string_without_last_hexa2(std::string &buffer)
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
void new_string_show2(std::string &buffer)
{
	size_t	i = 0, j = 0;
	for(;i < buffer.size();i++)
	{
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				////////std::cout << buffer<<std::endl;
				buffer.erase(0,i + 1);
				////////std::cout << buffer<<std::endl;
				j+= 1;
				if(j > 0)
					break;
			}
		}

	}
}
void new_string_without_first_hexa2(std::string &buffer)
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
	// int find = buffer.find(fill.c_str(),0);
	//////std::cout << find<<std::endl;
	buffer = buffer.substr(fill.size(),buffer.size());
}
void new_string_without_first_and_last_v22(std::string &buffer)
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
void new_string_without_first_and_last2(std::string &buffer)
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
std::string extractFieldName2(const std::string& header) {
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
std::string extractFieldFileName2(const std::string& header) {
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
void new_string_erase_content_disposition2(std::string &buffer)
{
	//size_t	i = 0;
	//int found_n = 0;
	if(buffer.find("filename") == std::string::npos)
		buffer = buffer.erase(0,34);
	else
		buffer = buffer.erase(0,32);
	std::string name = extractFieldName2(buffer);
	size_t i = 6 + name.size() + 1;
	// //////std::cout << buffer<<std::endl;
	buffer = buffer.erase(0,i);
	// //////std::cout << buffer<<std::endl;
	if(buffer.find("ilename",0) != std::string::npos)
	{
		std::string ilename = extractFieldFileName2(buffer);
		i = 10 + ilename.size() + 3;
		buffer.erase(0, i);
		// //////std::cout << buffer<<std::endl;

	}
}
void new_string_erase_content_type2(std::string &buffer)
{
	size_t find = buffer.find("Content-Type");
	if(find != std::string::npos)
	{
		size_t find_n = buffer.find("\n",find);
		buffer.erase(find,find_n - find + 1);
	}
}
unsigned long int convert_hexa(std::string hexa)
{
	char* endPtr; // Used to detect invalid conversions
    unsigned long int result = strtoul(hexa.c_str(), &endPtr, 16);
	return result;
}
bool isHexadecimal(const std::string &str)
{
    std::stringstream ss(str); // to read integer(hexa) from a string
    int n;
    ss >> std::hex >> n; // to extract the integers one by one
    return !ss.fail() && ss.eof();
    // we use func fail() to check the extration opr performed by ss >> ..; was succ or not
}
//find hexadecimal
void find_hexa(std::string &buffer, std::string &returnaa)
{
	//////std::cout <<"----------------------------------------------------\n"
	// << buffer;
	//////std::cout << "##########################################\n" ;
	size_t pos = 0;
	std::string token;
	returnaa = " ";
	while ((pos = buffer.find("\r\n")) != std::string::npos)
	{
		token = buffer.substr(0, pos);
		////////std::cout << token<<std::endl;
		if (isHexadecimal(token))
		{
			if (token == "0")
			{
				// requestbuffer.close();
				// set_has_request(true);
				//////std::cout << "show yourself"<<std::endl;
				returnaa = token;
				return ;
			}
			returnaa = buffer.substr(0,pos);
			buffer = buffer.erase(0, pos + 2);
			return ;
		}
		buffer = buffer.erase(0, pos + 2);	
	}
}
void find_hexa_v2(std::string &buffer, std::string &returnaa)
{
	size_t pos = 0;
	std::string token;
	returnaa = " ";
	while ((pos = buffer.find("\r\n")) != std::string::npos)
	{
		token = buffer.substr(0, pos);
		////////std::cout << token<<std::endl;
		if (isHexadecimal(token))
		{
			if (token == "0")
			{
				// requestbuffer.close();
				// set_has_request(true);
				//////std::cout << "show yourself"<<std::endl;
				returnaa = token;
				return ;
			}
			returnaa = buffer.substr(0,pos);
			buffer = buffer.erase(0, pos + 2);
			return ;
		}
		buffer = buffer.erase(0, pos + 2);	
	}
}
// check hexadecimal


void new_string_buff_12(std::string &buffer)
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

void	getFilename(std::string buffer, int file_index, struct uploadFiles* upload_files, int find, int fd,struct client &clt)
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
	// //////std::cout << clt.temp_header<<std::endl;
	if (size && find_cr != -1)
	{

		temp = buffer.substr(find, find_cr - find);
		find_filename = temp.find("filename=");
		if (find_filename != -1)
			find_filename = buffer.find("filename=", find);
		upload_files->no_name = "no";
	}
	else if (size)
		find_filename = buffer.find("filename=", find);
	upload_files->no_name = "no";
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
		upload_files->filename = "./www/uploads/" + temp;
		upload_files->just_the_file = temp;
		upload_files->no_name = "no";
	}
	else if(clt.temp_header[0] != 0) {
		temp = clt.temp_header;
		size_t content_di = temp.find("Content-Disposition");
		(void)content_di;
		find_filename = temp.find("filename=");
		find_filename += 9;
		int j = 0;
		while ((size_t)find_filename + j < temp.size() && temp[find_filename + j] != '\"')
			j++;
		temp = temp.substr(find_filename,j);
		upload_files->filename = "./www/uploads/" + temp;
		upload_files->just_the_file = temp;
		upload_files->no_name = "no";

	}
	else 
	{
		std::srand(static_cast<unsigned>(std::time(nullptr)));
		int a = std::rand();
		char r[1000000];
		std::sprintf(r, "%d", a);
		temp = "text_" + std::string(r) +std::to_string(file_index);
		upload_files->filename = "./www/uploads/" + temp;
		upload_files->just_the_file = temp;
		upload_files->no_name = "yes";
	}
}

// void	multiTypes(std::string buffer,std::string &buffer2, struct client& clt , int n)
// {
// 	(void)buffer;
// 	(void)buffer2;
// 	(void)n;

// 	if (clt.nbr_of_reads == 1)
// 	{
// 		//////std::cout <<YELLOW <<  buffer;
// 		//////std::cout  << RED <<  "----------------------------------\n";
// 		buffer = buffer.substr(buffer.find("\r\n\r\n") + 4, buffer.size());
// 	}
// 	if (!clt.map_request["Transfer-Encoding"].empty())
// 	{
		
// 	}
// 	//////std::cout << SKY << buffer << END;
// 	if (!clt.map_request.empty())
// 		//////std::cout << "------------####-$$$$$$$$$$-@@@@@@@-----------------\n";
// 	clt.position += n;
// }

void	multiTypes(std::string buffer,std::string &buffer2, struct client& clt , int n)
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
	(void)n;
	theEnd = "\r\n\r\n" + clt.bodys.boundary;
	flag_postman_curl = 1;
	size = buffer.size();
	i = 0;
	if (clt.bodys.content_length_flag == 1 && clt.bodys.chunks_flag == 0 && clt.bodys.boundary_flag == 0)
	{
		//std::cout << YELLOW << "no chunks and no boundary\n" << END;
		find = 0;
		if (clt.nbr_of_reads == 1)
		{
			// //////std::cout << BLUE<<clt.map_request["Content-Disposition"]<<std::endl<<END;
			find_cr = buffer.find("\r\n\r\n");
			if(find_cr != -1)
				clt.temp_header =  buffer.substr(0,find_cr + 4);
			// //////std::cout << clt.temp_header<< std::endl;
			if (find_cr != -1)
			{
				buffer = buffer.substr(find_cr + 4);
				buffer2 = buffer;
			}
		}
		i = clt.upload_files.size();
		if (buffer.size() && i == 0)
		{
			////////std::cout << clt.nbr_of_reads<<std::endl;
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
			getFilename(buffer, clt.upload_files.size(), &upload_files, -1, clt.fd,clt);
			upload_files.file->open(upload_files.filename.c_str(),std::fstream::app | std::fstream::out);
			//sleep(2);
			if (upload_files.file->is_open() == true)
			{
				upload_files.file->close();
				clt.upload_files.push_back(upload_files);
				clt.upload_files[0].no_name = upload_files.no_name;
			}
		}
		i = clt.upload_files.size();
		if (i)
		{
			temp.assign("");
			temp = clt.upload_files[i - 1].filename;
			clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::app | std::fstream::out);
			//////std::cout << "   "<<clt.bodys.content_len;
			//////std::cout << "   "<<buffer.size();
			if (buffer.size() >= clt.bodys.content_len)
				clt.upload_files[i - 1].file->write(buffer.c_str(), clt.bodys.content_len);
			else
			{
				clt.upload_files[i - 1].file->write(buffer.c_str(), buffer.size());
			}
			clt.upload_files[i - 1].file->close();
		}
	}
	if (clt.bodys.chunks_flag == 1)
	{
		int 			flag;
		std::string		bndry;
		std::string		tmp;
		unsigned long	hex;
		unsigned long	i;

		flag = 1;
		if (clt.bodys.boundary_flag == 1)
		{
			bndry = clt.bodys.boundary.substr(0, clt.bodys.boundary.size() - 2);
		}
		else
			bndry = "";
		while (buffer.size())
		{
			find = 0;
			if (clt.nbr_of_reads == 1 && flag)
			{
				find_cr = buffer.find("\r\n\r\n");
				if (clt.bodys.boundary_flag == 1)
					find = buffer.find(bndry, find_cr);
				else
					find = -1;
				if (find != -1)
				{
					find = buffer.find("Content-Disposition:");
					if (find != -1 && isADerective(buffer, find, size) == 1 && clt.bodys.content_disposition == 0)
					{
						getFilename(buffer, clt.upload_files.size(), &upload_files, find, clt.fd,clt);
						upload_files.file->open(upload_files.filename.c_str(), std::fstream::app | std::fstream::out);
						if (upload_files.file->is_open() == true)
						{
							upload_files.file->close();
							clt.upload_files.push_back(upload_files);
							clt.upload_files[0].no_name = upload_files.no_name;
						}
						find_cr = buffer.find("\r\n\r\n", find);
						buffer = buffer.substr(find_cr + 6);
						clt.bodys.content_disposition = 1;
						buffer2 = buffer;
					}
				}
				else
				{
					getFilename(buffer, clt.upload_files.size(), &upload_files, find, clt.fd,clt);
					upload_files.file->open(upload_files.filename.c_str(), std::fstream::app | std::fstream::out);
					if (upload_files.file->is_open() == true)
					{
						upload_files.file->close();
						clt.upload_files.push_back(upload_files);
						clt.upload_files[0].no_name = upload_files.no_name;
					}
					buffer = buffer.substr(find_cr + 4);
					clt.bodys.content_disposition = 1;
					buffer2 = buffer;
				}
				flag = 0;
			}
			if (clt.nbr_of_reads > 1 &&  clt.bodys.plan_hex == false)
			{
				clt.bodys.tmp_buff += buffer;
				buffer.assign(clt.bodys.tmp_buff); 
				clt.bodys.tmp_buff.assign("");
			}
			i = 0;
			if (clt.bodys.left_bytes == 0)
			{
				while (i < buffer.size() && isHex(buffer[i]) && buffer[i] != '\r')
				{
					i++;
				}
				if (buffer[i] == '\r' && i + 1 <= buffer.size() && buffer[i + 1] == '\n')
					clt.bodys.plan_hex = true;
				else
				{
					clt.bodys.plan_hex = false;
					clt.bodys.tmp_buff.assign(buffer);
					return ;
				}
			}
			else
			{
				hex = clt.bodys.left_bytes; 
				clt.bodys.left_bytes = 0;
			}
			if (i)
			{
				hex = hexToDec(buffer.substr(0, i));
				if (i + 2 < buffer.size())
					buffer = buffer.substr(i + 2, buffer.size());
			}
			if (buffer.size() && hex)
			{
				if (clt.bodys.boundary_flag == 1)
				{
					int index = buffer.find(clt.bodys.boundary);
					if (index == 0 || index == 2)
					{
						buffer.assign("");
						return ;
					}
				}
				std::string temp = clt.upload_files[0].filename;
				clt.upload_files[0].file->open(temp.c_str(), std::fstream::app | std::fstream::out);
				if (clt.upload_files[0].file->is_open() == true)
				{
					if (buffer.size() < hex)
					{
						clt.bodys.left_bytes = hex - buffer.size();
						hex = buffer.size();
					}
					tmp = buffer.substr(0, hex );
					clt.upload_files[0].file->write(tmp.c_str(), tmp.size());
					clt.upload_files[0].file->close();
				}
				if (hex + 2 < buffer.size())
				{
					buffer = buffer.substr(hex + 2, buffer.size());
				}
				else
					buffer.assign("");
			}
			else if (hex == 0)
				buffer.assign("");
		}
	}
	else if  (clt.bodys.boundary_flag == 1)
	{
		//std::cout << GREEN << "no chunks and boundary\n" << END;
		int 			flag;
		std::string		bndry;
		std::string		tmp;

		flag = 1;
		bndry = clt.bodys.boundary.substr(0, clt.bodys.boundary.size() - 2);
		// //////std::cout << buffer<<"    ll"<< std::endl;
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
					buffer2 = buffer;
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
						////////std::cout << "Enter 1"<<std::endl;
						getFilename(buffer, clt.upload_files.size(), &upload_files, find, clt.fd,clt);
						upload_files.file->open(upload_files.filename.c_str(), std::fstream::app | std::fstream::out);
						if (upload_files.file->is_open() == true)
						{
							upload_files.file->close();
							clt.upload_files.push_back(upload_files);
							clt.upload_files[0].no_name = upload_files.no_name;
						}
						find_cr = buffer.find("\r\n\r\n", find);
						buffer = buffer.substr(find_cr + 4);
						clt.bodys.content_disposition = 1;
					}
					else if (clt.bodys.content_disposition == 0)
					{
						////////std::cout << "Enter 2"<<std::endl;
						if (clt.upload_files.size() == 0)
						{
							getFilename(buffer, clt.upload_files.size(), &upload_files, find, clt.fd,clt);
							upload_files.file->open(upload_files.filename.c_str(), std::fstream::app | std::fstream::out);
							if (upload_files.file->is_open() == true)
							{
								upload_files.file->close();
								clt.upload_files.push_back(upload_files);
								clt.upload_files[0].no_name = upload_files.no_name;
							}
							clt.bodys.content_disposition = 1;
						}
						else 
							clt.bodys.content_disposition = 1;
					}
					if (clt.bodys.content_disposition == 1)
					{
						////////std::cout << "Enter 5"<<std::endl;
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
							////////std::cout << "Enter 6"<<std::endl;
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
						////////std::cout << "Enter 7"<<std::endl;
						buffer.assign("");
					}
				}
			}
		}
	}
	// else if(clt.bodys.chunks_flag == 1)
	// {
	// 	//std::cout << SKY << "chunks and boundary ? or chunks and no boundary\n" << END;
	// 	int 			flag;
	// 	std::string		bndry;
	// 	std::string		tmp;
	// 	std::string		temp;

	// 	flag = 1;
	// 	bndry = clt.bodys.boundary.substr(0, clt.bodys.boundary.size() - 2);
		

	// 	while (buffer.size() > 0)
	// 	{
	// 		find = 0;
	// 		if (clt.nbr_of_reads == 1 && flag)
	// 		{
	// 			find_cr = buffer.find("\r\n\r\n");
	// 			find = buffer.find(bndry, find_cr);
	// 			if (find != -1)
	// 			{
	// 				buffer = buffer.substr(find);
	// 				buffer2 = buffer;
	// 			}
	// 			flag = 0;
	// 		}
	// 		else
	// 		{
	// 			if(clt.bodys.first_hexa == 0)
	// 			{
	// 				find_hexa(buffer,clt.bodys.returnaa);
	// 				if(clt.bodys.returnaa == "")
	// 					return ;
	// 				clt.bodys.len_chunk = convert_hexa(clt.bodys.returnaa);
	// 				//////std::cout << clt.bodys.len_chunk<<std::endl;
	// 				clt.bodys.first_hexa = 1;
	// 				clt.bodys.how_much_read_stay = clt.bodys.len_chunk;
	// 				getFilename(buffer, clt.upload_files.size(), &upload_files, find, clt.fd,clt);
	// 				upload_files.file->open(upload_files.filename.c_str(), std::fstream::binary | std::fstream::app | std::fstream::out);
	// 				if (upload_files.file->is_open() == true)
	// 				{
	// 					upload_files.file->close();
	// 					clt.upload_files.push_back(upload_files);
	// 					clt.upload_files[0].no_name = upload_files.no_name;
	// 				}
	// 			}
	// 			else if(clt.bodys.first_hexa == 1)
	// 			{
	// 				if(clt.bodys.has_read_all_chunk == true)
	// 				{
	// 					tmp = buffer;
	// 					find_hexa(tmp,clt.bodys.returnaa);
	// 					clt.bodys.has_read_all_chunk = false;
	// 				}
	// 				if(clt.bodys.how_much_read_stay == 0)
	// 					clt.bodys.how_much_read_stay = clt.bodys.len_chunk;
	// 				i = clt.upload_files.size();
	// 				if(clt.bodys.returnaa == "0")
	// 				{
	// 					//////std::cout << "give me some sign"<<std::endl;
	// 					temp.assign("");
	// 					temp = clt.upload_files[i - 1].filename;
	// 					temp = buffer.substr(0,buffer.size());
	// 					clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::binary | std::fstream::app | std::fstream::out);
	// 					clt.upload_files[i - 1].file->write(temp.c_str(), temp.size());
	// 					clt.upload_files[i - 1].file->close();
	// 					buffer.erase(0,clt.bodys.len_chunk);
	// 					//buffer.erase(0,buffer.size());
	// 					//////std::cout << buffer.size();
	// 					clt.bodys.len_chunk = 0;
	// 				}
	// 				else if (buffer.size() && clt.bodys.how_much_read_stay == clt.bodys.len_chunk)
	// 				{
	// 					////////std::cout << "Enter 6"<<std::endl;
	// 					//////std::cout << "Enter here"<<std::endl;
	// 					temp.assign("");
	// 					temp = clt.upload_files[i - 1].filename;
	// 					clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::binary | std::fstream::app | std::fstream::out);
	// 					if(buffer.size() >  clt.bodys.len_chunk)
	// 					{
	// 						temp = buffer.substr(0, clt.bodys.len_chunk);
	// 						buffer.erase(0,clt.bodys.len_chunk);
	// 						clt.bodys.has_read_all_chunk = true;
	// 						tmp = buffer;
	// 						find_hexa(tmp,clt.bodys.returnaa);
	// 					}
	// 					else
	// 					{
	// 						temp = buffer.substr(0, buffer.size());
	// 						clt.bodys.how_much_read_stay = clt.bodys.len_chunk - buffer.size();
	// 						buffer.erase(0,buffer.size());
	// 					}
	// 					//////std::cout << "size = " << buffer.size()<<std::endl;
	// 					clt.upload_files[i - 1].file->write(temp.c_str(), temp.size());
	// 					clt.upload_files[i - 1].file->close();
	// 				}
	// 				else if(buffer.size() && clt.bodys.how_much_read_stay != clt.bodys.len_chunk && clt.bodys.how_much_read_stay != 0)
	// 				{
	// 					//////std::cout << "Enter here2"<<std::endl;

	// 					temp.assign("");
	// 					temp = clt.upload_files[i - 1].filename;
	// 					clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::binary | std::fstream::app | std::fstream::out);
	// 					if(buffer.size() >  clt.bodys.how_much_read_stay)
	// 					{
	// 						temp = buffer.substr(0, clt.bodys.how_much_read_stay);
	// 						buffer.erase(0,clt.bodys.how_much_read_stay);
	// 						clt.bodys.has_read_all_chunk = true;
	// 						tmp = buffer;
	// 						find_hexa(tmp,clt.bodys.returnaa);
	// 					}
	// 					else
	// 					{
	// 						temp = buffer.substr(0, buffer.size());
	// 						clt.bodys.how_much_read_stay = clt.bodys.how_much_read_stay - buffer.size();
	// 						buffer.erase(0,buffer.size());
	// 						buffer.assign("");
	// 					}
	// 					clt.upload_files[i - 1].file->write(temp.c_str(), temp.size());
	// 					clt.upload_files[i - 1].file->close();
	// 					buffer.erase(0,clt.bodys.how_much_read_stay);
	// 					clt.bodys.has_read_all_chunk = true;
	// 					tmp = buffer;
	// 					find_hexa(tmp,clt.bodys.returnaa);
	// 				}

	// 			}
	// 			// ////////std::cout << buffer<<std::endl;
	// 			// //sleep(10);

	// 		// 				if (buffer.size() >= clt.bodys.content_len)
	// 		// 	clt.upload_files[i - 1].file->write(buffer.c_str(), clt.bodys.content_len);
	// 		// else
	// 		// {
	// 		// 	clt.upload_files[i - 1].file->write(buffer.c_str(), buffer.size());
	// 		// }
	// 			// if(i)
	// 			// {
	// 			// 	if(clt.bodys.len_chunk == 0)
	// 			// 		return ;
	// 			// 	//////std::cout << "give me some sign"<<std::endl;
	// 			// 	temp.assign("");
	// 			// 	temp = clt.upload_files[i - 1].filename;
	// 			// 	clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::binary | std::fstream::app | std::fstream::out);
	// 			// 	temp = buffer.substr(0, clt.bodys.len_chunk);
	// 			// 	clt.upload_files[i - 1].file->write(temp.c_str(), temp.size());
	// 			// 	clt.upload_files[i - 1].file->close();
	// 			// 	// buffer = buffer.erase(0,clt.bodys.len_chunk);
	// 			// 	// //////std::cout << buffer.size();
	// 			// 	// clt.bodys.len_chunk = 0;
	// 			// }
	// 			// else if (buffer.size() && i)
	// 			// {
	// 			// 		////////std::cout << "Enter 6"<<std::endl;
	// 			// 	//////std::cout << "Enter here"<<std::endl;
	// 			// 	temp.assign("");
	// 			// 	temp = clt.upload_files[i - 1].filename;
	// 			// 	clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::binary | std::fstream::app | std::fstream::out);
	// 			// 	temp = buffer.substr(0, buffer.size());
	// 			// 	clt.upload_files[i - 1].file->write(temp.c_str(), temp.size());
	// 				clt.upload_files[i - 1].file->close();
	// 				buffer = buffer.assign("");
	// 			}
			 
	// 	}
	// }
	
}
// }
