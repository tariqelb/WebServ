/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleConnectinon.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:04:07 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/04/01 23:34:17 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

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
			if (FD_ISSET(web.servers[i].socketFd[j], &web.tmp))
			{
					newClient.len = sizeof(newClient.addr);
					newClient.fd = accept(web.servers[i].socketFd[j], (struct sockaddr *)&newClient.addr, &newClient.len);
					printf("New connection accepted with fd :  %d\n", newClient.fd);
					if (newClient.fd < 0)
					{
						write(2, "Error : Fail connecting to an client\n", 37);
						return ;
					}
					else
					{
						FD_SET(newClient.fd, &web.cReads);
						printf("client fd %d\n", newClient.fd);
						web.clients.push_back(newClient);
					}
					printf("is_set %d\n", web.servers[i].socketFd[j]);
			}
			j++;
		}
		i++;
	}
	std::cout << "End of handle connection" << std::endl;
}
