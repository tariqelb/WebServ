/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recieveRequest.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 11:40:12 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/16 17:06:39 by tel-bouh         ###   ########.fr       */
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
    struct body                     bodys;

    bodys.chunks_flag = 0;
    bodys.boundary_flag = 0;
    bodys.content_length_flag = 0;
    bodys.cr_nl_flag = 0;
    bodys.chunks_len = 0;
    bodys.chunks_con_len = 0;
    bodys.content_len = 0;
    bodys.n_chunks = 0;
    bodys.cr_index = -1;
	memset(line, 0, 100000);
	n_byte_readed = 0;
	
	n_byte_readed = recv(clt.fd, line, 99999, 0);
	line[n_byte_readed] = 0;
	if (n_byte_readed < 0)
	{
		std::cerr << "Error in recv" << std::endl;
		closeConnection(web, clt_i);	
		return ;
	}
	std::cout << "Read bytes : " << n_byte_readed << std::endl;
	if (n_byte_readed == 0)
	{
		clt.request_is_ready = true;
		return ;
	}
	else
		clt.buffer << line;
	//std::cout << "line [" << line  << "]"<< std::endl;
	if (endOfTheRequest(clt.buffer.str(), bodys) == 0)
	{
		std::cout << "End the request\n";
		clt.request_is_ready = true;
	}
}
