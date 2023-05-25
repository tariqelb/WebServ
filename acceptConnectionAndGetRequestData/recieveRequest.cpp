/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 11:40:12 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/25 23:45:22 by tel-bouh         ###   ########.fr       */
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

	memset(line, 0, 100000);
	n_byte_readed = 0;
	n_byte_readed = recv(clt.fd, line, 99999, 0);
	line[n_byte_readed] = 0;
	if (n_byte_readed < 0)
	{
		closeConnection(web, clt_i);	
		return ;
	}
	if (n_byte_readed == 0)
	{
		clt.request_is_ready = true;
		return ;
	}
	else
	 	clt.buffer.write((char *) line, n_byte_readed); //	clt.buffer << line;
	if (endOfTheRequest(clt.buffer.str(), clt.bodys) == 0)
	{
		//std::cout << "Buffer [" << clt.buffer.str() << "]" << clt.buffer.str().size() << std::endl;
		std::fstream s;
		s.open("name.txt", std::ios::trunc | std::ios::out);
		if (!s.is_open())
		{
			std::cout << errno << std::endl;
			std::cout << "Not opned " << std::endl;
		}
		s << clt.buffer.str();
		s.close();
		clt.request_is_ready = true;
		std::cout << "[" << clt.buffer.str() << "]" << std::endl;
		FD_SET(clt.fd, &web.writes);
	}
}
