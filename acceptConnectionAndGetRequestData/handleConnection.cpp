/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleConnectinon.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:04:07 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/16 17:07:34 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

char    temp[2048] = "HTTP/1.0 200 OK\r\n Server: webserver-c\r\n Content-type: text/    html\r\n\r\n <html>hello, world    </html>\r\n";


void	closeConnection(struct webserv& web, int client_i)
{
   	std::vector<client>::iterator it;

	it = web.clients.begin();
	FD_CLR(web.clients[client_i].fd , &web.reads);
	FD_CLR(web.clients[client_i].fd , &web.writes);
	close(web.clients[client_i].fd);
	//std::cout << "1 here " << web.clients.size() << " , index : " << client_i << std::endl;
   	//std::cout << "itr : " << (*it).fd << " and " << web.clients[client_i].fd << std::endl;
	while (client_i < web.clients.size() && (*it).fd != web.clients[client_i].fd && it != web.clients.end())
		it++;
	//std::cout << "2 here " << web.clients.size() << " , index : " << client_i << std::endl;
   	//std::cout << "itr : " << (*it).fd << " and " << web.clients[client_i].fd << std::endl;
	web.clients.erase(it);
	maxFd(web);
}

void	handleConnection(struct webserv& web)
{
	struct client 	newClient;
	int				i;
	int				j;
	int				size;

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
						std::cerr << "Error : Fail connecting to client" << std::endl;
						return ;
					}
					else
					{
						std::cout  << "New client fd : " << newClient.fd << std::endl;
						FD_SET(newClient.fd, &web.reads);
						FD_SET(newClient.fd, &web.writes);
						web.clients.push_back(newClient);
						maxFd(web);
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
			std::cout << "The new client : " << web.clients[i].fd << " nbr of clients : " <<  web.clients.size() << std::endl;
			receiveRequest(web, web.clients[i], i);
		}
		i++;
	}
	
	i = 0;
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_write) )
		{
			if (web.clients[i].request_is_ready == true)
			{
				send(web.clients[i].fd, temp, strlen(temp), 0);
				closeConnection(web, i);
				std::cout << "----------------------------------------------" << std::endl;
			}
			else
				std::cout << "Client not ready " << std::endl;
		}
		i++;
	}
}
