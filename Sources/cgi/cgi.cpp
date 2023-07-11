/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:05:26 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/11 14:15:32 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"
#include <unistd.h>

void	fill_CGI_ENV(struct client &clt, struct webserv &web)
{
	clt.cgi.env.push_back("QUERY_STRING=" + clt.map_request["QUERY_STRING"]);
	clt.cgi.env.push_back(std::string("REMOTE_ADDR=" + web.config[clt.config].host));
	clt.cgi.env.push_back(std::string("REQUEST_METHOD=GET"));
	clt.cgi.env.push_back(std::string("SCRIPT_NAME=") + clt.map_request["URI"]);
	clt.cgi.env.push_back("SERVER_PORT=" + clt.port);
	clt.cgi.env.push_back(std::string("SCRIPT_FILENAME=") + clt.map_request["URI"]);
	clt.cgi.env.push_back("HTTP_HOST="+clt.map_request["Host"]);
	clt.cgi.env.push_back("HTTP_USER_AGENT=" + clt.map_request["User-Agent"]);
	if(clt.map_request["Method"] == "POST")
	{
		clt.cgi.env.push_back("CONTENT_TYPE=" + clt.map_request["CONTENT-TYPE"]);
		clt.cgi.env.push_back("CONTENT_LENGTH=" + clt.map_request["CONTENT-LENGTH"]);
	}
}

void	executeCgi(struct client &clt,CGI &cgi, std::string &filePath)
{
	int fd[2];
	pid_t pid;
	if (pipe(fd))
	{
		std::cerr << "Error : pipe\n";
		return ;
	}

	if ((pid = fork()) == -1)
	{
		std::cerr << "Error : fork\n";
		return ;
	}
	if (!pid)
	{
		if (close (fd[0]) == -1)
			std::cerr << "Error closing fd[0]\n";
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		
		const char* arg[] = {filePath.c_str(), NULL};
		char **env = new char *[cgi.env.size() + 1];
		int i;
		for (i = 0; i < cgi.env.size(); i++)
			env[i] = const_cast<char*>(cgi.env[i].c_str());
		env[i] = NULL;
		// if(clt.map_request["Method"] == "POST")
		// {
		// 	close(STDIN_FILENO);
		// 	if(dup2(STDOUT_FILENO,STDIN_FILENO) == -1)
		// 	{
		// 		std::cerr << "Error: dup2\n";
		// 		exit(0);
		// 	}
		// }
		if (execve(filePath.c_str(), const_cast<char**>(arg), env) == -1)
		{
			std::cerr << "Error: execve\n";
			exit(0);
		}
	}
	waitpid(pid, 0, 0);
	std::fstream out;
	out.open("out.html", std::ios::out);
	if (!out.is_open())
	{
		std::cerr << "Error: out file\n";
		return ;
	}
	ssize_t bytesRead;
	struct stat fileState;
	fstat(fd[0], &fileState);
	std::cout << "fileState.st_size = " << fileState.st_size << std::endl;
	std::vector<char> buffer(static_cast<int>(fileState.st_size));
	if ((bytesRead = read(fd[0], buffer.data(), buffer.size())) > 0)
		out << buffer.data();
	else if  (bytesRead == -1) 
	{
		std::cerr << "Error reading from fd[0]\n";
		return;
	}
	close(fd[0]);
	out.close();
	filePath = "out.html";
	out.close();
	close (fd[1]);
}

int isCgiConfigured(struct client &clt, struct webserv &web, std::string &extention)
{
	std::vector<std::pair<std::string, std::string> >::iterator iter;
	for (iter = web.config[clt.config].location[clt.location].cgi.begin();
		iter != web.config[clt.config].location[clt.location].cgi.end()
		&& iter->second != clt.map_request["URI"];
		iter++);
	if (iter == web.config[clt.config].location[clt.location].cgi.end())
	{
		std::cout << "I did not find any thing\n";
		return 0;
	}
	// extention = iter->first;
	// if(extension == ".py")
	// 	clt.cgi.interpreter = "/usr/bin/python";
	// if(extension == ".cpp")
	// 	clt.cgi.interpreter = "/usr/bin/python";
	// if(extension == ".sh")
	// 	clt.cgi.interpreter = "/bin/bash";
	// if(extension == ".php")
	// 	clt.cgi.interpreter = "/usr/bin/php";
	return 1;
}



int cgi(struct webserv &web, struct client &clt)
{
	std::string extention;
	std::string original_file_path = clt.map_request["URI"];
	if (!isCgiConfigured(clt, web, extention))
		return 0;
	fill_CGI_ENV(clt, web);
	executeCgi(clt,clt.cgi, clt.map_request["URI"]);
	std::ifstream CGI_file;
	CGI_file.open("out.html");
	if(!CGI_file)
		std::cerr<< "Error opening the file"<<std::endl;
	std::string line;
	std::string buffer = "";
	while(getline(CGI_file,line))
	{
		buffer += line;
	}
	CGI_file.close();
	clt.cgi.buffer = buffer;
	clt.cgi.original_file_path = original_file_path;
	return clt.response.statusCode = 200;
}