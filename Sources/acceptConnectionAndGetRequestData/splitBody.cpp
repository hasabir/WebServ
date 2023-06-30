/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitBody.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 22:54:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/29 12:50:30 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"
/*
void	addHeaders(std::string buffer, struct client& clt, int pos)
{
	int find;

	find = buffer.find("\r\n\r\n", pos);
	if (find != -1)
	{
		clt.headers = clt.headers + buffer.substr(pos, find - pos + 2);
	}
}
*/
void	splitBody(std::string buffer, struct client& clt)
{
	int 					size;
	int 					find;

	find = -1;
	size = buffer.size();
	if (size == 0)
		return ;
	if (clt.nbr_of_reads == 1 && clt.headers.size() == 0)
	{
		find = buffer.find("\r\n\r\n");
		if (find > 0)
		{
			clt.headers = buffer.substr(0, find + 2);
		}
	}
	if (clt.post_flag)
	{
		if (clt.bodys.get_body_type == 0)
			getBodyType(buffer, clt.bodys);
		multiTypes(buffer, clt);
	}
}
