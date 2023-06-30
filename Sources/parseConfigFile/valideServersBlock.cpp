/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valideServersBlock.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:30:09 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/29 12:50:30 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../webserv.hpp"

int	valideServerName(std::string line)
{
	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line.compare(i, 6, "server") != 0)
		return (0);
	i = i + 6;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (i != size)
		return (0);
	return (1);
}

int	isAclosedServer(std::vector<std::string> file, int line_index)
{
	int i;
	int	size;
	int open_bracket;

	i = line_index + 1;
	open_bracket = 1;
	while (i < size && open_bracket)
	{
		if (isBracket(file[i]) == 1)
			open_bracket++;
		else if (isBracket(file[i]) == 2)
			open_bracket--;
		else if (endsWithSemiColon(file[i]) == 0)
		   return (-1);
		i++;
	}
	if (open_bracket != 0)
		return (-1);
	return (i);
}


int	valideServersBlock(std::vector<std::string> file)
{
	int i;
	int j;
	int	size;
	int open_brackets;

	open_brackets = 0;
	size = file.size();
	i = 0;
	while (i < size)
	{
		open_brackets = 0;
		if (valideServerName(file[i]))
		{
			i++;
			if (isBracket(file[i]) == 1)
				open_brackets = 1;
			else
				return (0);
			i++;
			while (i < size && isServerBlock(file[i]) == 0 && open_brackets)
			{
				int b = isBracket(file[i]);
				if (b == 1)
					open_brackets++;
				else if (b == 2)
					open_brackets--;
				else if (endsWithSemiColon(file[i]) == 0 && isLocationBlock(file[i]) == 0)
					return (0);
				i++;
			}
			if (open_brackets != 0)
				return (0);
		}
		else if (i < size && isEmptyLines(file[i]) != 0)
			return (0);
		else
			i++;
	}
	return (1);
}
