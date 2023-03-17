/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleConnectinon.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:04:07 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/17 11:04:58 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	handleConnection(struct webserv& web)
{
	struct client 	newClient;
	int				i;
	
	i = 0;
	while (i < MAX_PORT)
	{
		if (FD_ISSET(web.socketFd[i], &web.tmp))
		{
				newClient.len = sizeof(newClient.addr);
				newClient.fd = accept(web.socketFd[i], (struct sockaddr *)&newClient.addr, &newClient.len);
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
				printf("is_set %d\n", web.socketFd[i]);
		}
		i++;
	}
}
