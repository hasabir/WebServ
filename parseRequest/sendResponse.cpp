/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/02 19:51:05 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void fillResponseHeader(struct client &clt, struct webserv &web, int statusCode)
{
	std::string response;
	std::fstream file;
	response = "Connection: close\r\nServer: webserver-c\r\n "; 
	if (clt.location < 0 || web.config[clt.config].error_page.empty())
	{
		response += " Content-type: text/html\r\n";
	}
}


// int sendResponse(struct client &clt, struct webserv &web, int statusCode)
// {
// 	fillResponseHeader(clt, web, statusCode);
// }

int send_404(struct client &clt) {
	clt.response_is_ready = true;
	const char *c404 = "HTTP/1.1 404 Not Found\r\n"
		"Connection: close\r\n"
		"Content-Length: 9\r\n\r\nNot Found";
	send(clt.fd , c404, strlen(c404), 0);
	// closeConnection(web, i);
	return 404;
	// drop_client(client);
}