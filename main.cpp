/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 19:47:52 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/23 21:12:08 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int ac, char **av)
{
	struct	webserv		web;
	struct	timeval		tv;
	int					index;
	int					flag;

	index = 0;
	tv.tv_sec = 120;
	tv.tv_usec = 0;
	//get data from config file
	web.status = 0;
	parseConfigFile(web, ac, av);
	if (web.status != 0)
	{
		std::cerr << "No valid server found" << std::endl;
		return (1);
	}
	//Fill  struct
	initWebStrcut(web);
	if (web.status != 0)
	{
		std::cerr << "Error : init webserv struct" << std::endl;
		return (1);
	}
	//Display IP and PORT
	displayHostPort(web);
	//Launch servers
	flag = initServer(web);
	if (flag)
	{
		std::cerr << "Error : initialing webserv" << std::endl;
		freedWeb(web);
		return (1);
	}
	//Set select for incomming connections
	activeSocket(web);
	while (1)
	{
		web.status = -1;
		FD_ZERO(&web.tmp_read);
		FD_ZERO(&web.tmp_write);
		web.tmp_read = web.reads;
		web.tmp_write = web.writes;
		std::cout << "Wait in select : " << web.clients.size() << std::endl;
		web.status = select(web.maxReadFd + 1, &web.tmp_read, &web.tmp_write, 0, &tv);
		if (web.status == 0)
		{
			std::cerr << "server time listening out" << std::endl;
			freedWeb(web);
			return (1);
		}
		if (web.status < 0)
		{
			std::cerr << "Error : Fail occur on select function : " << web.status << std::endl; 
			freedWeb(web);
			return (1);

		}
		else //(web.status == 0)
			handleConnection(web);
	//	handleRequest(web);	
	}	
	freedWeb(web);
	
	return (0);
}
