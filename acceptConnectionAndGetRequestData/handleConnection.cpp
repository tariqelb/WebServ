/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleConnectinon.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:04:07 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/15 14:04:37 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

char    temp[2048] = "HTTP/1.0 200 OK\r\n Server: webserver-c\r\n Content-type: text/    html\r\n\r\n <html>hello, world    </html>\r\n";

void	handleConnection(struct webserv& web)
{
	struct client 	newClient;
	int				i;
	int				j;
	int				size;
	std::vector<client>::iterator   it;

	size = web.servers.size();	
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			if (FD_ISSET(web.servers[i].socketFd[j], &web.tmp_read))
			{
					newClient.len = sizeof(newClient.addr);
					newClient.fd = accept(web.servers[i].socketFd[j], (struct sockaddr *)&newClient.addr, &newClient.len);
					std::cout << "New connection accepted with fd : " << newClient.fd << std::endl;//remove
					if (newClient.fd < 0)
					{
						std::cerr << "Error : Fail connecting to an client" << std::endl;
						return ;
					}
					else
					{
						FD_SET(newClient.fd, &web.reads);
						if (newClient.fd > web.maxReadFd)
							web.maxReadFd =  newClient.fd;
						web.clients.push_back(newClient);
					}
			}
			j++;
		}
		i++;
	}
	size = web.clients.size();	
	i = 0;
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_read))
		{
			std::cout << "The new client : " << web.clients[i].fd << " Size : " <<  web.clients.size() << std::endl;
			receiveRequest(web, web.clients[i], i);
		}
		i++;
	}
	
	i = 0;
	while (i < size)
	{
		//if (FD_ISSET(web.clients[i].fd, &web.tmp_write()) )
		if (web.clients[i].request_is_ready == true)
		{
			it = web.clients.begin();
			send(web.clients[i].fd, temp, strlen(temp), 0);
			closeConnection(web, it, i);
			std::cout << "remove client : " << web.clients.size() << std::endl;
		}
		i++;
	}
}
