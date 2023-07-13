/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:05:26 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/13 16:41:05 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

// void	executeCgi(struct client &clt,CGI &cgi, std::string &filePath)
// {
// 	int fd[2];
// 	pid_t pid;
	// if (pipe(fd))
	// {
	// 	std::cerr << "Error : pipe\n";
	// 	return ;
	// }

// 	if ((pid = fork()) == -1)
// 	{
// 		std::cerr << "Error : fork\n";
// 		return ;
// 	}
// 	if (!pid)
// 	{
// 		// if (close (fd[0]) == -1)
// 		// 	std::cerr << "Error closing fd[0]\n";
// 		dup2(fd[1], STDOUT_FILENO);
// 		close(fd[1]);
		
// 		const char* arg[] = {filePath.c_str(), NULL};
// 		char **env = new char *[cgi.env.size() + 1];
// 		int i;
// 		for (i = 0; i < cgi.env.size(); i++)
// 			env[i] = const_cast<char*>(cgi.env[i].c_str());
// 		env[i] = NULL;
// 		// if(clt.map_request["Method"] == "POST")
// 		// {
// 		// 	close(STDIN_FILENO);
// 		// 	if(dup2(STDOUT_FILENO,STDIN_FILENO) == -1)
// 		// 	{
// 		// 		std::cerr << "Error: dup2\n";
// 		// 		exit(0);
// 		// 	}
// 		// }
// 		if (execve(filePath.c_str(), const_cast<char**>(arg), env) == -1)
// 		{
// 			std::cerr << "Error: execve\n";
// 			exit(0);
// 		}
// 	}
// 	waitpid(pid, 0, 0);
// 	std::fstream out;
// 	out.open("out.html", std::ios::out);
// 	if (!out.is_open())
// 	{
// 		std::cerr << "Error: out file\n";
// 		return ;
// 	}
// 	ssize_t bytesRead;
// 	struct stat fileState;
// 	fstat(fd[0], &fileState);
// 	// std::cout << "fileState.st_size = " << fileState.st_size << std::endl;
// 	std::vector<char> buffer(static_cast<int>(fileState.st_size));
// 	if ((bytesRead = read(fd[0], buffer.data(), buffer.size())) > 0)
// 		out << buffer.data();
// 	else if  (bytesRead == -1) 
// 	{
// 		std::cerr << "Error reading from fd[0]\n";
// 		return;
// 	}
// 	close(fd[0]);
// 	out.close();
// 	filePath = "out.html";
// 	out.close();
// 	close (fd[1]);
// }


void	executeCgi(struct client &clt,CGI &cgi, std::string &filePath)
{
	pid_t pid;
	std::ofstream out("out.html");

	int fd_out = open("out.html", O_RDWR, 644);
	if (!out.is_open())
	{
		std::cerr << "Error: out file\n";
		return ;
	}
	if ((pid = fork()) == -1)
	{
		std::cerr << "Error : fork\n";
		return ;
	}
	if (!pid)
	{
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
		
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
	out.close();
	filePath = "out.html";
}

int cgi(struct webserv &web, struct client &clt)
{
	std::string extention;
	int			status;
	std::string original_file_path = clt.map_request["URI"];

	if ((status = isCgiConfigured(clt, web, extention, original_file_path)) != 1)
		return status;
	fill_CGI_ENV(clt, web);
	executeCgi(clt,clt.cgi, clt.map_request["URI"]);
	return clt.response.statusCode = 200;
}