/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 11:40:12 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/08 18:35:10 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	maxFd(struct webserv& web)
{
	int i;
	int j;
	int size_s;
	int	size_c;
	int size_fd;
	int max;

	max = -1;
	i = 0;
	size_s = web.servers.size();
	size_c = web.clients.size();
	while (i < size_s)
	{
		j = 0;
		size_fd = web.servers[i].socketFd.size();
		while (j < size_fd)
		{
			if (web.servers[i].socketFd[j] > max)
				max = web.servers[i].socketFd[j];
			j++;
		}
		i++;
	}
	i = 0;
	while (i < size_c)
	{
		if (web.clients[i].fd > max)
			max = web.clients[i].fd;
		i++;
	}
	web.maxReadFd = max;
}

void	receiveRequest(struct webserv& web, struct client& clt, int clt_i)
{
	char							line[100000];
	int 							i;
	int								n_byte_readed;
	std::string						buff;

	memset(line, 0, 100000);
	n_byte_readed = 0;
	n_byte_readed = recv(clt.fd, line, 99999, 0);
	line[n_byte_readed] = 0;
	buff.assign("");
	//std::cout << "the size of rd : "  << n_byte_readed << std::endl;
	//std::cout << "The request : " << line << std::endl;
	if (n_byte_readed < 0)
	{
		closeConnection(web, clt_i);	
		return ;
	}
	if (n_byte_readed == 0)
	{
		if (clt.nbr_of_reads == 0)
		{
			//std::cout << "Empty request " << std::endl;
			FD_CLR(web.clients[i].fd , &web.reads);
			closeConnection(web, clt_i);
		}
		clt.request_is_ready = true;
		return ;
	}
	else
	{
		if (clt.nbr_of_reads == 0)
			clt.file->open(clt.file_name.c_str(),  std::fstream::out);
		else
			clt.file->open(clt.file_name.c_str(),  std::fstream::app | std::fstream::out);
		if (!clt.file->is_open())
			std::cerr << "Can not open file." << std::endl;
		//you need to check for file is opned and doing what the sutiation needs
		clt.file->write((char *) line, n_byte_readed); //	clt.buffer << line;
		clt.file->close();
		clt.bodys.rd_bytes = clt.bodys.rd_bytes + n_byte_readed;
		buff.assign(line, n_byte_readed);
		//std::cout << "the size of each : " << buff.size() << " " << n_byte_readed << std::endl;
		//std::cout << "The request : " << buff << std::endl;
		if (clt.nbr_of_reads == 0)
		{	
			if (buff.find("POST") == 0)
			{
				clt.post_flag = 1;
			}
		}
		clt.nbr_of_reads++;
	}
	if (clt.post_flag == 1)
		splitBody(buff, clt);
	if (endOfTheRequest(buff, clt.bodys) == 0)
	{
		clt.request_is_ready = true;
		//std::cout << "Headers : [" << clt.headers << "]" << std::endl;  
		FD_SET(clt.fd, &web.writes);
	}
}
