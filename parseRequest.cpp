/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 21:03:09 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/22 22:07:15 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


int	isWhileSpace(int c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == ':')
		return (1);
	return (0);
}

int	isRequestWellFormed(struct webserv web, std::string buff)
{
	return (1);
}

void	getMethodUriVersion(struct client& clt, std::string buff)
{
	int i;
	int size;
	std::pair<std::string, std::string> pr;

	//Get Method -----------------------------------------
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = "Method";
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);
	pr.first.assign("");
	pr.second.assign("");
	while (i < size && isWhileSpace(buff[i]))
		i++;
	buff = buff.substr(i, buff.size());
	//Get Uri  -----------------------------------------
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = "Uri";
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);
	pr.first.assign("");
	pr.second.assign("");
	while (i < size && isWhileSpace(buff[i]))
		i++;
	buff = buff.substr(i, buff.size());
	//Get Http -----------------------------------------
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = "Http";
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);

}
void	getHostPort(struct client& clt, std::string buff)
{
	int i;
	int size;
	std::pair<std::string, std::string> pr;

	//Get Host and Port -------------------------------------
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = buff.substr(0, i);
	while (i < size && isWhileSpace(buff[i]))
		i++;
	buff = buff.substr(i, buff.size());
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);
	pr.first.assign("");
	pr.second.assign("");
	while (i < size && isWhileSpace(buff[i]))
		i++;
	buff = buff.substr(i, buff.size());
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = "Port";
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);
}

void	getKeyValue(struct client& clt, std::string buff)
{
	int find;
	int size;
	int i;
	int j;
	std::pair<std::string, std::string> pr;
		
	size = buff.size();
	find = buff.find(":");
	if (find > 0)
	{
		pr.first = buff.substr(0, find);
		i = find + 1;
		while (i < size && (buff[i] == ' ' || buff[i] == '\t'))
			i++;
		j = 0;
		while (i + j < size && buff[i] != '\n')
			j++;
		pr.second = buff.substr(i, j);
		clt.request.push_back(pr); 
	}
}

void	fillRequestData(struct client& clt, std::stringstream& buffer)
{
	int										i;
	int 									size;
	std::pair<std::string, std::string>		pr;
	std::string 							buff;
	int										rd;

	i = 0;
	rd = 0;
	size = buffer.str().size();
	getline(buffer, buff);
	rd += buff.size() + 1;
	while (rd < size)
	{
		if (i == 0)
			getMethodUriVersion(clt, buff);
		else if (buff.compare(0, 6, "Host: ") == 0)
			getHostPort(clt, buff);
		else if (buff == "\r")
		{
			//get the body
			//Create another body key val if you reach max std::string size;
			getline(buffer, buff);
			rd += buff.size() + 1;
			pr.first = "Body";
			while (rd < size)
			{
				buff += "\n";
				pr.second += buff;
				buff.assign("");
				getline(buffer, buff);
				rd += buff.size() + 1;
			}
			clt.request.push_back(pr);
			return ;
		}
		else if (buff.size())
			getKeyValue(clt, buff);
		//std::cout << "["  << buff.substr(0, buff.size() - 1) << "]" << std::endl;
		buff.assign("");

		getline(buffer, buff);
		rd += buff.size() + 1;
		i++;
	}
}

void	parseRequest(struct webserv& web, struct client& clt)
{
//	std::cout << "****************************************" << std::endl;
//	std::cout << clt.buffer.str() << std::endl;
//	std::cout << "****************************************" << std::endl;
//	fillRequestData(clt, clt.buffer);
	/*int i = 0;
	int size = clt.request.size();
	std::cout << "The size : " <<  size << std::endl;
	while (i < size)
	{
		//if (clt.request[i].first == "Body")
		{
			std::cout << "Key : " << clt.request[i].first;
			std::cout << ", val : " << clt.request[i].second  << std::endl;
		}
		i++;
	}*/
	/*if (isRequestWellFormed(web, buff) == 0)
	{
		std::cerr << "Request not well formed" << std::endl;
	}*/
}
