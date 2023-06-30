/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitSemiColons.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:48:44 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/29 12:50:30 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	removeDoubleSemiColons(std::string& line)
{
	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size)
	{
		if (line[i] == ';')
		{
			int j = i + 1;
			while (j < size && (line[j] == ' ' || line[j] == '\t'))
				j++;
			if (j > i && line[j] == ';')
			{
				line[i] = ' ';
				i = -1;
			}
		}
		i++;
	}
}

void	splitSemiColons(std::vector<std::string>& file)
{
	int i;
	int size;
	int find;
	int	flag;
	std::vector<std::string>::iterator it;
	
	flag = 0;
	it = file.begin();
	size = file.size();
	i = 0;
	while (i < size)
	{
		removeDoubleSemiColons(file[i]);
		find = file[i].find(";");
		if (find >= 0)
		{
			flag = file[i].find("server");
			if (flag > 0 && (file[i][flag + 6] == ' ' || file[i][flag + 6] == '\t' || file[i][flag + 6] == 0))
				;
			else
			{	
				int len = file[i].size();
				int j = find + 1;
				while (j < len && file[i][j] == ' ' && file[i][j] == '\t')
					j++;
				if (j < len)
				{
					file.insert(it + i + 1, file[i].substr(find + 1, len - find - 1));
					file[i].assign(file[i].substr(0, find + 1));
					it = file.begin();
					size = file.size();
				}
			}
		}	
		i++;
	}
}




int	isAloneSemiColon(std::string  line)
{
	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == ';')
		i++;	
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (i == size)
		return (1);
	return (0);
}


void	removeEmptySemiColons(std::vector<std::string>& file)
{
	int i;
	int size;
	std::vector<std::string>::iterator it;

	it = file.begin();
	size = file.size();
	i = 0;
	while (i < size)
	{
		if (isServerBlock(file[i]))
		{
			i++;
			while (i < size && !isServerBlock(file[i]))
			{
				if (isAloneSemiColon(file[i]))
				{
					file.erase(it + i);
					it = file.begin();
					size = file.size();
				}
				else
					i++;
			}
		}
		else
			i++;
	}
}





