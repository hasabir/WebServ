/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 19:45:42 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/01 19:46:21 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

unsigned long stringToInt(std::string str)
{
	long long n(0);
	std::string::iterator iter(str.begin());

	for (; iter != str.end() && std::isdigit(*iter); iter++)
		n = n * 10 + *iter - '0';
	if (iter != str.end() || str.empty())
	{
		if (*iter == 'M')
			n *= 1000000;
		else if (*iter == 'k')
			n *= 1000;
	}
	return n;
}