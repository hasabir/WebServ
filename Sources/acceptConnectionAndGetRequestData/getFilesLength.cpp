/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getFilesLength.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 17:02:44 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/29 12:50:30 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"


void	getFilesLength(struct client& clt)
{
	int					i;
	int					size;
	unsigned long		total_length;
	unsigned long		file_length;
	std::string			filename;

	
	i = 0;
	size = clt.upload_files.size();
	total_length = 0;
	while (i < size)
	{
		filename = clt.upload_files[i].filename;
		clt.upload_files[i].file->open(filename.c_str(), std::fstream::in);
		if (clt.upload_files[i].file->is_open())
		{
			clt.upload_files[i].file->seekg(0, clt.upload_files[i].file->end);
			file_length = clt.upload_files[i].file->tellg();
			total_length = total_length + file_length;
			clt.upload_files[i].file->close();
		}
		i++;
	}
	clt.body_length = total_length;
}
