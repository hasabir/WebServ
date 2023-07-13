/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:05:26 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/13 19:34:42 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	executeCgi(struct client &clt,CGI &cgi, std::string &filePath)
{
	pid_t pid;
	std::fstream out("out.html");

	int fd_out = open("out.html", O_RDWR, 0644);
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
		env[i] = nullptr;

		if (execve(cgi.interpreter.c_str(), const_cast<char**>(arg), env) == -1)
		{
			std::cerr << "Error: execve\n";
			exit(0);
		}
	}
	waitpid(pid, 0, 0);
	if (cgi.extention == ".php")
	{
		std::string lineToParse;
		getline(out, lineToParse, '\r');
		std::cout << "------------------>line = " << lineToParse << std::endl;
	}
	out.close();
	filePath = "out.html";
}


int cgi(struct webserv &web, struct client &clt)
{
	std::string extention;
	int			status;

	if ((status = isCgiConfigured(clt, web, clt.map_request["URI"])) != 1)
		return status;
	fill_CGI_ENV(clt, web);
	executeCgi(clt,clt.cgi, clt.map_request["URI"]);
	return clt.response.statusCode = 200;
}