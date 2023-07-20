/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:05:26 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/20 13:40:28 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	generate_CGI_file(struct client &clt,std::string &filePath)
{
	if (clt.cgi.extention == ".php")
		filePath = parsePHPcgi(clt.cgi.outFile, clt.cgi.header, intToString(clt.fd));
	else if (clt.cgi.extention == ".py" && !clt.cgi.loop_detected)
	{
		filePath = "out" + intToString(clt.fd) +".html";
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
	clt.cgi.outFile = "out" +intToString(clt.fd) + ".html";
	int fd_out = open(clt.cgi.outFile.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_out == -1)
		throw std::runtime_error("Error1: opening file\n");
		if ((clt.cgi.pid = fork()) == -1)
			throw std::runtime_error("Error: fork\n");
		if (!clt.cgi.pid)
		{
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);
	
			if(clt.map_request["Method"] == "POST")
			{
				std::ifstream in(clt.upload_files[0].filename.c_str());
				getline(in, clt.map_request["QUERY_STRING"], '\0');
				std::cerr << YELLOW << "clt.map_request = " << clt.map_request["QUERY_STRING"] << END << std::endl;
				in.close();
				int fd = open(clt.upload_files[0].filename.c_str(), O_RDWR, 0644);
				if (fd < 0)
					throw std::runtime_error("Error: failed to open input file");
				dup2(fd, 0);
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
		return 0;
	std::cout << YELLOW << "---------> "<< clt.map_request["Method"] << END << std::endl;
	try {
			executeCgi(clt, clt.map_request["URI"], web);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return error(clt, 500);
	}
	if (!clt.response.body)
		return clt.response.statusCode = 200;
	return 0;
}