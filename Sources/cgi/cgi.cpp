/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:05:26 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/15 13:01:06 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

std::string	parsePHPcgi(std::string fileName)
{
	std::ifstream	in(fileName);
	std::string 	responseFileName;
	std::string		content_type;
	std::fstream 	out;
	std::string 	line;

	if (!in.is_open())
		throw std::runtime_error("Error: opening file\n");
	while (getline(in, line,'\n') && line != "\r")
	{
		if ((line.find("Content-type")) != std::string::npos)
		{
			int start = line.find(':') + 2;
			int end = line.find(';');
			if (end == std::string::npos)
				end = line.length() - 1;
			content_type = line.substr(start, end - start);
		}
	}
	std::map<std::string, std::string> contentTypes;
	std::map<std::string, std::string>::iterator iter;
	
	fillMapContentTypes(contentTypes);
	for (iter = contentTypes.begin(); iter != contentTypes.end(); iter++)
	{
		if (iter->second == content_type)
		{
			responseFileName = "cgi" + iter->first;
			if (std::remove("cgi.html")< 0)
				throw std::runtime_error("Error: remove");
			break;
		}
	}
	if (iter == contentTypes.end())
		responseFileName = "cgi.html";
	out.open(responseFileName, std::ios::out);
	while (getline(in, line))
		out << line << std::endl;
	out.close();
	in.close();
	return responseFileName;
}


void	executeCgi(struct client &clt,CGI &cgi, std::string &filePath)
{
	pid_t pid;
	std::fstream out("out.html");

	int fd_out = open("out.html", O_RDWR | O_TRUNC, 0644);
	if (!out.is_open())
	{
		std::cerr << "Error: out file\n";
		return;
	}
	if ((pid = fork()) == -1)
	{
		std::cerr << "Error : fork\n";
		return;
	}
	if (!pid)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);

		const char* arg[] = {cgi.interpreter.c_str() , filePath.c_str(), NULL};
		char** env = new char*[cgi.env.size() + 2];  //!
		int i;
		for (i = 0; i < cgi.env.size(); i++)
			env[i] = const_cast<char*>(cgi.env[i].c_str());
		env[i++] = const_cast<char*>("REDIRECT_STATUS=1");  //! for php
		env[i] = NULL;

		if(clt.map_request["Method"] == "POST")
		{
			if(dup2(STDOUT_FILENO,STDIN_FILENO) == -1)
			{
				std::cerr << "Error: dup2\n";
				exit(0);
			}
		}
		if (execve(cgi.interpreter.c_str(), const_cast<char**>(arg), env) == -1)
		{
			std::cerr << "Error: execve\n";
			exit(0);
		}
	}
	out.close();
	waitpid(pid, 0, 0);
	if (cgi.extention == ".php")
	{
		filePath = parsePHPcgi("out.html");
		return ;
	}
	filePath = "cgi.html";
}


int cgi(struct webserv &web, struct client &clt)
{
	std::string extention;
	int			status;

	clt.response.cgi = true;
	if ((status = isCgiConfigured(clt, web, clt.map_request["URI"])) != 1)
		return status;
	fill_CGI_ENV(clt, web);
	executeCgi(clt,clt.cgi, clt.map_request["URI"]);
	return clt.response.statusCode = 200;
}