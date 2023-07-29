/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:48:15 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/27 17:16:42 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

std::string getContentTyp(std::string filePath)
{
	std::map<std::string, std::string> contentTypes;
	std::map<std::string, std::string>::iterator iter;
	std::string type;
	int index;
	
	fillMapContentTypes(contentTypes);
	index = filePath.find_last_of('.');
	if ((size_t)index == std::string::npos)
		return "application/octet-stream";
	type = filePath.substr(index, filePath.size());
	if ((iter = contentTypes.find(type)) != contentTypes.end())
		return iter->second;
	return "Not found";
}
unsigned long stringToUnsignedLong(const std::string& str) {
    unsigned long result = 0;

    for (size_t i = 0; i < str.length(); ++i) {
        char ch = str[i];
        if (ch >= '0' && ch <= '9') {
            result = result * 10 + static_cast<unsigned long>(ch - '0');
        } else {
            std::cerr << "Error: Invalid input string \"" << str << "\"\n";
            exit(1); // Or handle the error in your desired way
        }
    }

    return result;
}
std::map<std::string, std::string>::iterator content_type_exist(struct client& clt,std::string pp)
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
std::map<std::string, std::string>::iterator extensionNew(std::string old,std::map<std::string,std::string> ContentTypes)
{
	
	fillMapContentTypes(ContentTypes);
	std::map<std::string, std::string>::iterator it = ContentTypes.begin();
	for(;it != ContentTypes.end();it++)
	{
		if(it->second.compare(old) != 0)
			continue;
		else
			break;
	}
	if(it != ContentTypes.end())
		return (it);
	return (it);
}
int	post(struct webserv& web, struct client& clt)
{

	std::string path;
	std::string path2;
	std::string path3;

	struct stat pathStat1;
	struct stat pathStat2;
	struct stat pathStat3;
	if(clt.upload_files.size() <= 0)
		return error(clt,404);
	int place = web.config[clt.config].location[clt.location].upload_store.size() - 1;
	uploadFiles file = clt.upload_files[0];

	std::string temp22 = clt.upload_files[0].filename;
	std::string temp33 = temp22;
	std::string extension;
	std::ifstream file2;
	std::string comparitor = "no";
	std::string temp_str2;

	
	if(clt.bodys.chunks_flag == 1)
	{
		
		std::cout << stringToInt(web.config[clt.config].max_body_size)<<std::endl;
		std::cout << clt.body_length<<std::endl;
		if(clt.body_length >= stringToInt(web.config[clt.config].max_body_size))
			return (error(clt,413));
	}
	if(clt.upload_files[0].no_name.compare(comparitor.c_str()) == 0)
	{
		std::cout <<"Hereee 2"<<std::endl;
		extension = temp22.erase(0,14);
		bool only_one = false;
		std::string temp_original = temp33;
		size_t find_last_folder = clt.map_request["URI"].rfind("/");
		temp33 = temp33.erase(find_last_folder,temp33.size() - find_last_folder);
		if(temp33.rfind("/") == std::string::npos)
		{
			only_one = true;
			temp33 = temp_original + '/' + extension;
		}
		if(only_one == false)
			temp33 = temp33 + '/' + extension;
		file2.open(temp33,std::ios::in);
		file2.close();
		clt.upload_files[0].filename = extension;
		temp_str2 = "./www/uploads/" + clt.upload_files[0].filename;
		std::cout << temp33<<std::endl;
	}	
	else
	{
		std::string CTTYPE = "Content-Type";
		std::map<std::string,std::string> ContentTypes;
		temp_str2 =  clt.upload_files[0].filename;
		std::cout << "here"<<std::endl;
		temp22 = clt.map_request["URI"];
		std::cout << temp22<<std::endl;
		size_t find_last_fold = temp22.rfind("/");
		extension = temp22.erase(0,find_last_fold + 1);
		std::cout << extension<<std::endl;
		//temp22 = clt.map_request["URI"] + '/' + extension;
		std::ifstream file3;
		file3.open(clt.map_request["URI"],std::ios::in);
		file3.close();
		extension = clt.upload_files[0].filename.erase(0,14);
		clt.upload_files[0].filename = clt.map_request["URI"];
		if(content_type_exist(clt,CTTYPE) != clt.map_request.end())
		{
			if(extensionNew(content_type_exist(clt,CTTYPE)->second,ContentTypes) != ContentTypes.end())
				extension = extension + extensionNew(content_type_exist(clt,CTTYPE)->second,ContentTypes)->first;
		}
	}
	std::cout << clt.upload_files[0].just_the_file<< std::endl;
	if(stat(web.config[clt.config].location[clt.location].upload_store.c_str(),&pathStat1) == 0)
	{	
		if(clt.location >= 0 && web.config[clt.config].location[clt.location].upload == "on")
		{
			std::cout << "here 493"<<std::endl;
			// if(stat(clt.map_request["URI"].c_str(),&pathStat3) != 0)
			// 	return (error(clt,404));
			// if(S_ISDIR(pathStat3.st_mode))
			// 	return (error(clt,404));
			// std::ifstream original_file;
			// std::cout << "the file is "<< clt.map_request["URI"]<<std::endl;
			// original_file.open(clt.upload_files[0].filename.c_str(),std::ios::in);
			// if(original_file.is_open() == 0)
			// {
			// 	std::cerr<< "Error Opening the file\n"<<std::endl;
			// 	return  error(clt, 500);
			// }
			// original_file.close();

			if(clt.bodys.boundary_flag == 1)
			{
				std::string new_file_body = "";
				std::ifstream input3;
				std::ofstream out3;
				std::string line3;
				std::string CTTYPE = "Content-Type";
				std::map<std::string,std::string> ContentTypes;
				if(content_type_exist(clt,CTTYPE) != clt.map_request.end())
				{
					input3.open(temp_str2,std::ios::in);
					if(!input3.is_open())
					{
						std::cerr << "Error: input file\n";
						return  error(clt, 500);
					}
					std::string temp_content_type = clt.map_request["Content-Type"].erase(0,30);
					//std::cout << temp_content_type<<std::endl;
					int find_first = 0;
					while(getline(input3,line3))
					{
						if(line3.compare(temp_content_type) == 0 && find_first == 0)
						{
							find_first = 1;
							temp_content_type = temp_content_type + "--";
							continue;
						}
						if(line3.compare(temp_content_type) == 0)
						{
							continue;
						}
						new_file_body += line3;
						new_file_body += "\n";
					}
					std::cout << new_file_body << std::endl;
					input3.close();
					out3.open(temp_str2,std::ios::in);
					if(!out3.is_open())
					{
						std::cerr << "Error: input file\n";
						return  error(clt, 500);
					}
					out3 << new_file_body;
					out3.close();
				}
				else
					error(clt,403);

			}
			if(access(web.config[clt.config].location[clt.location].upload_store.c_str(),X_OK) == 0)
			{
				
				std::string temp;
				std::ifstream input;
				std::ofstream out;
				while (web.config[clt.config].location[clt.location].upload_store[place] == '/' && place != 1)
				{
					temp = web.config[clt.config].location[clt.location].upload_store.substr(0,place);
					std::cout << temp<<std::endl;
					web.config[clt.config].location[clt.location].upload_store = temp;
					place = web.config[clt.config].location[clt.location].upload_store.size() - 1;
				}
				std::cout << " "<<extension<<std::endl;
				std::string destination = web.config[clt.config].location[clt.location].upload_store + "/" + extension;
				std::cout << temp_str2<<std::endl;
				out.open(destination,std::ios::out);
				if(!out.is_open())
				{
					std::cerr << "Error: out file\n";
					return  error(clt, 500);
				}
				input.open(temp_str2,std::ios::in);
				if(!input.is_open())
				{
					std::cerr << "Error: input file\n";
					return  error(clt, 500);
				}
				std::string line;
				std::string all = "";
				while(getline(input,line))
				{
					all += line;
					all += "\n";
				}
				out << all;
				out.close();
				input.close();
				if (remove(temp_str2.c_str()) != 0) {
        			std::cerr << "Error deleting the original file: " << clt.upload_files[0].filename << std::endl;
        			return  error(clt, 500);
    			}
				std::ofstream out2;
				std::ifstream input2;
				out2.open("./www/rspnse.html",std::ios::out);
				if(!out2.is_open())
				{
					std::cerr << "Error: out file\n";
					return  error(clt, 500);
				}
				input2.open(destination,std::ios::in);
				if(!input2.is_open())
				{
					std::cerr << "Error: input file\n";
					return  error(clt, 500);
				}
				std::string line2;
				std::string all2 = "";
				while(getline(input2,line2))
				{
					all2 += line2;
					all2 += "\n";
				}
				out2 << all2;
				out2.close();
				input2.close();
				clt.map_request["URI"] = "./www/rspnse.html";// file the file 
				return clt.response.statusCode = 201;
			}
			else if(access(web.config[clt.config].location[clt.location].upload_store.c_str(),F_OK) != 0)
			{
				return error(clt,404);
			}
			else if(access(web.config[clt.config].location[clt.location].upload_store.c_str(),X_OK) != 0 )
			{
				return error(clt,403);
			}
		}	
	}

	if(stat(clt.map_request["URI"].c_str(),&pathStat2) != 0)
		return (error(clt,404));

	std::cout<< clt.map_request["URI"].c_str()<<"Built "<<std::endl;
	if(S_ISDIR(pathStat2.st_mode))
	{
		
		if(*clt.map_request["URI"].rbegin() != '/')
		{
			clt.map_request["URI"] += "/";
			clt.response.body = true;
			clt.response.statusCode = 301;
		}
		if(*clt.map_request["URI"].rbegin() == '/')
		{
			if(clt.location >= 0 && web.config[clt.config].location[clt.location].index.empty())
				return error (clt,403);
			else if(clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
			{
				path2 = clt.map_request["URI"] + web.config[clt.config].location[clt.location].index;
				if(web.config[clt.config].location[clt.location].cgi.empty())
				{
					return (error(clt,403));
				}
				else
				{
					std::cout << "Wtp"<<std::endl;
					std::cout << path2 << "  "<<std::endl;
					if(!stat(path2.c_str(),&pathStat3))
					{			
						
						if(!S_ISDIR(pathStat3.st_mode))
						{
							std::cout << "Wtp"<<std::endl;
							clt.map_request["URI"] = path2;
							if ( cgi(web,clt))
								return 0;
							return (error(clt,403));
						}
						else
							return (error(clt,403));
					}
					else
						return (error(clt,403));
				}
			}
		}
	}
	if (!S_ISDIR(pathStat2.st_mode))
	{
		std::cout << "File"<<std::endl;
		if(web.config[clt.config].location[clt.location].cgi.empty())
		{
			return (error(clt,403));
		}
		else
		{	
			std::cout << " Here"<<std::endl;
			if ( cgi(web,clt))
				return 0;
			return (error(clt,403));
		}
	}
	return clt.response.statusCode = 500;
}