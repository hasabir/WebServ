/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:05:26 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/27 13:09:05 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	generate_CGI_file(struct client &clt,std::string &filePath)
{
	if (clt.cgi.extention == ".php")
	{
		clt.response.remove = true;
		filePath = parsePHPcgi(clt.cgi.outFile, clt.cgi.header, intToString(clt.fd));
	}
	else if (clt.cgi.extention == ".py" && !clt.cgi.loop_detected)
	{
		clt.response.remove = true;
		clt.cgi.outFile = "out" + intToString(clt.fd) +".html";
		filePath = clt.cgi.outFile;
	}
	else if (clt.cgi.extention == ".py" && clt.cgi.loop_detected)
	{
		kill(clt.cgi.pid, SIGKILL);
		error(clt, 508);
		return ;
	}
	clt.response.statusCode = 200;
}

void	executeCgi(struct client &clt, std::string &filePath, struct webserv &web)
{
	(void)web;
	clt.cgi.outFile = "out" +intToString(clt.fd) + ".html";
	int fd_out = open(clt.cgi.outFile.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_out == -1)
		throw std::runtime_error("Error1: opening file\n");
		if ((clt.cgi.pid = fork()) == -1)
			throw std::runtime_error("Error: fork\n");
		if (!clt.cgi.pid)
		{
			dup2(fd_out, STDOUT_FILENO);
			dup2(fd_out,2);
			close(fd_out);
			if(clt.map_request["Method"] == "POST")
			{
				int fd = open(clt.upload_files[0].filename.c_str(), O_RDONLY, 0777);
				if (fd < 0)
					throw std::runtime_error("Error: failed to open input file");
				if (dup2(fd, 0) < 0)
					throw std::runtime_error("Error: failed to duplicate file");
				if (close(fd))
					throw std::runtime_error("Error: failed to close input file");
			}
			fill_CGI_ENV(clt, web);
			const char* arg[] = {clt.cgi.interpreter.c_str() , filePath.c_str(), NULL};
			char** env = new char*[clt.cgi.env.size() + 2];  //!
			size_t i;
			for (i = 0; i < clt.cgi.env.size(); i++)
				env[i] = const_cast<char*>(clt.cgi.env[i].c_str());
			env[i++] = const_cast<char*>("REDIRECT_STATUS=1");  //! for php
			env[i] = NULL;
			if (execve(clt.cgi.interpreter.c_str(), const_cast<char**>(arg), env) == -1)
			{
				std::cerr << "Error: execve\n";
				exit(0);
			}
		}
		sleep(1);
		int returnValue = waitpid(clt.cgi.pid, NULL, WNOHANG);
		if (returnValue < 0)
			throw std::runtime_error("Error: wait failed");
		else if (returnValue)
		{
			generate_CGI_file(clt, filePath);
			return ;
		}
		else
		{
			clt.cgi.loop_detected = true;
			clt.cgi.time = 0;
			return;
		}
}


int cgi(struct webserv &web, struct client &clt)
{
	std::string extention;
	int			status;

	clt.response.cgi = true;
	if ((status = isCgiConfigured(clt, web, clt.map_request["URI"])) != 1)
	{
		return 0;
	}
	try {
			executeCgi(clt, clt.map_request["URI"], web);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return error(clt, 500);
	}
	if (!clt.response.body)
		return clt.response.statusCode = 200;
	return 0;
}