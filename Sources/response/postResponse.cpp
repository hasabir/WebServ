/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:48:15 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/20 13:50:24 by hasabir          ###   ########.fr       */
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
int	post(struct webserv& web, struct client& clt)
{

	//return  error(clt, 500);
	std::string path;
	std::string path2;
	std::string path3;
	// bool entered = false;
	struct stat pathStat1;
	struct stat pathStat2;
	struct stat pathStat3;
	// struct stat pathStat3;
	// struct stat pathUpload;
	// // 	std::map<std::string, std::string>::iterator iter;
	// // int i = 0;
	// // for (iter = clt.map_request.begin(); iter != clt.map_request.end();i++, iter++) {
    // //     std::cout << "\033[92m" <<  iter->first << " | " << iter->second << "\033[00m\n";
    // // }


	// // size_t pos = web.config[clt.config].location[clt.location].pattern.rfind('/');
	// // size_t pos_first = web.config[clt.config].location[clt.location].pattern.find('/');
	// // size_t pos_temp = 0;
	// // std::string substring = "";
	// // std::string temp_substr = "";
    // // while (pos > pos_first) 
	// // {
	// // 	substring = web.config[clt.config].location[clt.location].pattern.substr(0,pos);
	// // 	pos_temp = substring.rfind('/');
	// // 	if(temp_substr.empty())
	// // 		temp_substr = web.config[clt.config].location[clt.location].pattern.substr(pos) + temp_substr;
	// // 	else
	// // 	{
	// // 		pos = substring.rfind('/');
	// // 		if(pos <= pos_first)
	// // 			break;
	// // 		temp_substr = substring.substr(pos) + temp_substr;
	// // 	}
	// // 	entered = true;
	// // }
	// // if(pos <= pos_first && entered == false)
    // //      substring = "";
		
	// // path = clt.map_request["URI"] + temp_substr;
	if(clt.upload_files.size() <= 0)
		return error(clt,404);
	int place = web.config[clt.config].location[clt.location].upload_store.size() - 1;
	uploadFiles file = clt.upload_files[0];
	// return  error(clt, 500);
	std::cout << file.no_name <<std::endl;
	std::cout << file.filename<<std::endl;
	std::string temp22 = file.filename;
	std::string temp33 = temp22;
	std::string extension;
	std::ifstream file2;
	std::string comparitor = "no";
	std::cout <<"here"<<std::endl;
	if(file.no_name.compare(comparitor.c_str()) == 0)
	{
		// std::cout <<"Hereee 2"<<std::endl;
		// extension = temp22.erase(0,14);
		// bool only_one = false;
		// std::string temp_original = temp33;
		// size_t find_last_folder = clt.map_request["URI"].rfind("/");
		// temp33 = temp33.erase(find_last_folder,temp33.size() - find_last_folder);
		// if(temp33.rfind("/") == std::string::npos)
		// {
		// 	only_one = true;
		// 	temp33 = temp_original + '/' + extension;
		// }
		// if(only_one == false)
		// 	temp33 = temp33 + '/' + extension;
		// file2.open(temp33,std::ios::in);
		// if(file2.is_open() == 0)
		// {
		// 		std::cerr<< "Error Opening the file\n"<<std::endl;
		// 		return  error(clt, 500);
		// }
		// file2.close();
	}	
	else
	{
		std::cout << "here"<<std::endl;
		temp22 = clt.map_request["URI"];
		std::cout << temp22<<std::endl;
		size_t find_last_fold = temp22.rfind("/");
		extension = temp22.erase(0,find_last_fold + 1);
		// std::cout << extension<<std::endl;
		//temp22 = clt.map_request["URI"] + '/' + extension;
		std::ifstream file3;
		file3.open(clt.map_request["URI"],std::ios::in);
		if(!file3.is_open())
			return  error(clt, 500);
		file3.close();
	}

	if(stat(web.config[clt.config].location[clt.location].upload_store.c_str(),&pathStat1) == 0)
	{	
		if(clt.location >= 0 && web.config[clt.config].location[clt.location].upload == "on")
		{
			if(stat(clt.map_request["URI"].c_str(),&pathStat3) != 0)
				return (error(clt,404));
			if(S_ISDIR(pathStat3.st_mode))
				return (error(clt,404));
			std::ifstream original_file;
			original_file.open(clt.map_request["URI"].c_str(),std::ios::in);
			if(original_file.is_open() == 0)
			{
				std::cerr<< "Error Opening the file\n"<<std::endl;
				return  error(clt, 500);
			}
			original_file.close();
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
				std::string destination = web.config[clt.config].location[clt.location].upload_store + "/" + extension;
				out.open(destination,std::ios::out);
				if(!out.is_open())
				{
					std::cerr << "Error: out file\n";
					return  error(clt, 500);
				}
				input.open(file.filename,std::ios::in);
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
				std::cout << all <<std::endl;
				out << all;
				out.close();
				input.close();
				std::cerr << "I'm here"<<std::endl;
				if (remove(file.filename.c_str()) != 0) {
        			std::cerr << "Error deleting the original file: " << file.filename << std::endl;
        			return  error(clt, 500);
    			}
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
	// // if(stat(web.config[clt.config].location[clt.location].pattern.c_str(), &pathStat3) == 0)
	// // {
	// // 	if(S_ISDIR(pathStat3.st_mode))
	// // 	{
			
	// // 		// if(clt.location >= 0 && !web.config[clt.config].location[clt.location].redirect.empty())
	// // 		// {
	// // 		// 	if(clt.response.statusCode)
	// // 		// 		return clt.response.statusCode *= -1;
	// // 		// 	clt.response.body = true;
	// // 		// 	return clt.response.statusCode = 302;
				
	// // 		// }
	// // 		if(*web.config[clt.config].location[clt.location].pattern.rbegin() == '/')
	// // 		{
	// // 			if(clt.location >= 0 && web.config[clt.config].location[clt.location].index.empty())
	// // 				return error (clt,403);
	// // 			else if(clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
	// // 			{
	// // 				path2 =web.config[clt.config].location[clt.location].pattern + web.config[clt.config].location[clt.location].index;
	// // 				if(web.config[clt.config].location[clt.location].cgi.empty())
	// // 				{
	// // 					return (error(clt,403));
	// // 				}
	// // 				else
	// // 				{
	// // 					return cgi(web,clt);
	// // 				}
	// // 			}else if(clt.location < 0 && web.config[clt.config].index.empty())
	// // 			{
	// // 				path2 =web.config[clt.config].location[clt.location].pattern + web.config[clt.config].index;
	// // 			}
	// // 		}
	// // 		if(*web.config[clt.config].location[clt.location].pattern.rbegin()	!= '/')
	// // 		{
	// // 			clt.map_request["URI"] += "/";
	// // 			if(clt.location >= 0 && !web.config[clt.config].location[clt.location].redirect.empty())
	// // 				clt.response.body = true;
	// // 			return clt.response.statusCode = 301;
	// // 		}
	// // 	}
	// // 	if (!S_ISDIR(pathStat3.st_mode))
	// // 	{
	// // 		std::cout <<"i'm here"<<std::endl;
	// // 		if(web.config[clt.config].location[clt.location].cgi.empty())
	// // 		{
	// // 			return (error(clt,403));
	// // 		}
	// // 		else
	// // 		{
	// // 			return cgi(web,clt);
	// // 		}
	// // 	}
	// // }
	if(stat(clt.map_request["URI"].c_str(),&pathStat2) != 0)
		return (error(clt,404));
	std::cout<< clt.map_request["URI"].c_str()<<std::endl;
	if(S_ISDIR(pathStat2.st_mode))
	{
		
		// if(clt.location >= 0 && !web.config[clt.config].location[clt.location].redirect.empty())
		// {
		// 	if(clt.response.statusCode)
		// 		return clt.response.statusCode *= -1;
		// 	clt.response.body = true;
		// 	return clt.response.statusCode = 302;
		    
		// }
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
					return cgi(web,clt);
				}
			}else if(clt.location < 0 && web.config[clt.config].index.empty())
			{
				path2 = clt.map_request["URI"] + web.config[clt.config].index;
			}
		}
		if(*clt.map_request["URI"].rbegin() != '/')
		{
			clt.map_request["URI"] += "/";
			if(clt.location >= 0 && !web.config[clt.config].location[clt.location].redirect.empty())
				clt.response.body = true;
			return clt.response.statusCode = 301;
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
			std::cout << "Wtp"<<std::endl;
			return cgi(web,clt);
		}
	}
	return clt.response.statusCode = 500;
}