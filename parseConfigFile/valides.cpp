/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valides.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 18:39:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/29 12:56:36 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

int getNbr(std::string line)
{
	int i;
	int size;
	int nbr;

	nbr = 0;
	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
	while (i < size && line[i] >= '0' && line[i] <= '9')
	{
		nbr = nbr * 10;
		nbr = nbr + (line[i] - 48);
		i++;
	}
	return (nbr);
}

int	validePort(std::string line)
{
	int i;
	int size;
	int nbr;

	i = 0;
	size = line.size();
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
	while (i < size && (line[i] >= '0' && line[i] <= '9'))
		i++;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
	if (i != size)
		return (0);
	else
		nbr = getNbr(line);
	if (nbr >= 1024 && nbr <= 65535)
		return (1);
	return (0);
}

int	alphaDigit(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

int	valideDomainName(std::string line)
{
	int i;
	int size;

	i = 0;	
	size = line.size();
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == 'w')
	{
		int j = i + 3;
		while (i < j && i < size && line[i] == 'w')
			i++;
		if (line[i] == '.')
			i++;
	}
	if (line[i] == '-')
		return (0);
	while (i < size && line[i] != '.' && (alphaDigit(line[i]) || line[i] == '-'))
		i++;
	if (line[i] != '.')
		return (0);
	if (line[i] == '.' && line[i - 1] == '-')
		return (0);
	i++;
	int j = 0;
	while (i < size && alphaDigit(line[i]))
	{
		i++;
		j++;
	}
	if (j >= 2)
	{
		std::cout << "Valide servername "<< std::endl;
		return (1);
	}
	return (0);

}


int	valideFile(std::string line)
{
	std::ifstream file;
	std::string nbr;
	std::string name;
	int i;
	int size;
	int j;

	i = 0;
	size = line.size();
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	j = 0;
	while (i + j < size && line[i + j] != ' ' && line[i + j] != '\t')
		j++;
	nbr = line.substr(i, j);
	i = i + j;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	j = 0;
	while (i + j < size && line[i + j] != ' ' && line[i + j] != '\t')
		j++;
	name = line.substr(i, j);
	i = 0;
	size = nbr.size();
	while (i < size && nbr[i] >= '0' && nbr[i] <= '9')
		i++;
	if (i != size)
		return (0);
	file.open(name.c_str());
	if (file.is_open())
	{
		std::cout << "File opened\n";
		file.close();
		return (1);
	}
	std::cout << "File not  opened\n";
	return (0);
}


int valideUnit(std::string line)
{
	int nbr;
	int i;
	int size;
	char unit;
	
	i = 0;
	size = line.size();
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
	while (i < size && line[i] >= '0' && line[i] <= '9')
		i++;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
	if (line[i] == 'G' || line[i] == 'M' || line[i] == 'K')
	{
		unit = line[i];
		i++;
	}
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
	if (i != size)
		return (0);
	nbr = getNbr(line);
	if ((unit == 'G' || unit == 'K' || unit == 'M') && nbr > 0)
	{
		std::cout << "Valide Body size " << std::endl;
		return (1);
	}
	std::cout << "unvalide Body size " << std::endl;
	return 0;
}

int	valideOnOff(std::string line)
{
	int i;
	int size;

	i = 0;
	size = line.size();	
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
	if (i + 2 > size)
		return (0);
	if (line.compare(i, 2, "on") == 0)
	{
		if (line[i + 2] == 0)
		{
			std::cout << "Valide on value" << std::endl;
			return (1);
		}
		i += 2;
		while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (i == size)
		{
			std::cout << "Valide on value" << std::endl;
			return (1);
		}
		return (0);
	}	
	if (i + 3 > size)
		return (0);
	if (line.compare(i, 3, "off") == 0)
	{
		if (line[i + 3] == 0)
		{
			std::cout << "Valide off value" << std::endl;
			return (1);
		}
		i += 3;
		while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (i == size)
		{
			std::cout << "Valide off value" << std::endl;
			return (1);
		}
		return (0);
	}	
	std::cout << "Unvalide on/off value" << std::endl;
	return (0);
}

int	valideExtension(std::string line)
{
	int i;
	int j;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
	if (i < size &&  line[i] == '.')
		i++;
	else
		return (0);
	j = 0;
	while (i + j < size && isalpha(line[i + j]))
		j++;
	if (j == 0)
		return (0);
	i = i + j;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
	if (i == size)
	{
		std::cout << "Valide extension" << std::endl;
		return (1);
	}
	return (0);
}

int	valideScript(std::string line)
{
	int i;
	int j;
	int size;
	std::string name;

	i = 0;
	size = line.size();
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	j = 0;	
	while (i + j < size && line[i + j] != ' ' && line[i + j] != '\t')
		j++;
	if (j == 0)
		return (0);
	name = line.substr(i, j);
	i = i + j;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (i != size)
		return (0);
	j = access(name.c_str(), X_OK);
	if (j == 0)
	{
		std::cout << "Valide excutable" << std::endl;
		return (1);
	}
	return (0);
}

int	validePath(std::string line)
{
	int i;
	int j;
	int size;
	std::string path;
	struct stat buffer;

	i = 0;
	size = line.size();
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	j = 0;
	while (i + j < size && line[i + j] != ' ' && line[i + j] != '\t')
		j++;
	if (j == 0)
		return (0);
	path = line.substr(i, j);
	i = i + j;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (i != size)
		return (0);
	j = stat(path.c_str(), &buffer);
	if (j == 0)
	{
		if (S_ISDIR(buffer.st_mode))
		{
			std::cout << "Valide directory path" << std::endl;
			return (1);
		}
		return (0);
	}
	else
		return (0);
}
