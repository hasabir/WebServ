/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfigFun1.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:40:24 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/20 10:47:37 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


//check if a line have only whitespace characters return (0) if yes otherwise return (1)
int	isEmptyLines(std::string line)
{
	int	i;
	int size;

	size = line.size();
	i = 0;
	if (size == 0)
		return (0);
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (i == size)
		return (0);
	return (1);
}



//check if the line start with # so its a comment that will be removed
int	isComment(std::string line)
{
	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
	i++;
	if (line[i] == '#')
		return (1);
	return (0);
}

//check if a line starts with "server" so it will consider as start of server block
int	isServerBlock(std::string line)
{
	int find;
	int	i;

	find = line.find("server");
	if (find >= 0)
	{
		i = find + 6;//Index of the character after server word in the string
		if (find == 0 && (line[i] == ' ' || line[i] == '\t' || line[i] == 0 ))
			return (1);
		if (find > 0)
		{
			if ((line[find - 1] == ' ' || line[find - 1] == '\t') 
					&& (line[i] == ' ' || line[i] == '\t' || line[i] == 0))
				return (1);
		}
	}
	return (0);

/*	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line.compare(i, 6, "server") == 0)
	{
		if (line[i + 6] == ' ' || line[i + 6] == '\t' || line[i + 6] == 0)
			return (1);
	}
	return (0);*/
}
// check if a line starts with loaction block so its the start of locaion block
int	isLocationBlock(std::string line)
{
	int find;
	int	i;

	find = line.find("location");
	if (find >= 0)
	{
		i = find + 8;//Index of the character after server word in the string
		if (find == 0 && (line[i] == ' ' || line[i] == '\t' || line[i] == 0 ))
			return (1);
		if (find > 0)
		{
			if ((line[find - 1] == ' ' || line[find - 1] == '\t') 
					&& (line[i] == ' ' || line[i] == '\t' || line[i] == 0))
				return (1);
		}
	}
	return (0);

/*	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line.compare(i, 8, "location") == 0)
	{
		if (line[i + 8] == ' ' || line[i + 6] == '\t')
			return (1);
	}
	return (0);
	*/

}
//check if a line start with open or close curly brackets return (1) open return (2) close
int	isBracket(std::string line)
{
	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '{')
		return (1);
	if (line[i] == '}')
		return (2);
	return (0);

}

int	endsWithSemiColon(std::string line)
{
	int size;

	size = line.size() - 1;
	while (size >= 0 && (line[size] == ' ' || line[size] == '\t'))
		size--;
	if (line[size] == ';')
		return (1);
	return (0);
}
void	countServerBlock(std::vector<std::string> file, int& nbr_of_server)
{
	int 		i;
	int			size;

	size = file.size();
	i = 0;
	while (i < size)
	{
		if (isServerBlock(file[i]))
			nbr_of_server++;
		i++;
	}
}

int	nbrOfLocBlock(std::vector<std::string> file)
{
	int i;
	int	size;
	int count;

	size = file.size();
	i = 0;
	count = 0;
	while (i < size)
	{
		if (isLocationBlock(file[i]))
			count++;
		i++;
	}
	return (count);
}


int	closedBlock(std::vector<std::string> file)
{
	int i;
	int size;
	int	flag;
	int	nbr_of_loc_block;

	nbr_of_loc_block = 0;
	size = file.size();
	flag = 0;
	if (size > 3 && isServerBlock(file[0]) && isBracket(file[1]) == 1 && isBracket(file[size - 1]) == 2)
	{
		nbr_of_loc_block = nbrOfLocBlock(file);
		i = 0;
		while (i < size)
		{
			if (isLocationBlock(file[i]))
			{
				std::vector<std::string> loc;
				int k = i;
				while (k < size && isBracket(file[k]) != 2)
				{
					loc.push_back(file[k]);
					k++;
				}
				loc.push_back(file[k]);
				if (loc.size() > 3 && isBracket(loc[1]) == 1 && isBracket(loc[loc.size() - 1]) == 2)
					flag++;
			}
			i++;
		}
		if (nbr_of_loc_block == flag)
			return (1);
		return (0);
	}
	else
		return (0);
}
void	removeEmptyLineAndComments(std::vector<std::string>& file)
{
	std::vector<std::string>::iterator 	it;
	int									index;

	index = 0;
	it = file.begin();
	while (index < file.size())
	{
		if (isEmptyLines(file[index]) == 0)
		{
			file.erase(it + index);
			it = file.begin();
			index--;
		}
		else if (isComment(file[index]))
		{
			file.erase(it + index);
			it = file.begin();
			index--;
		}
		index++;
	}
}
