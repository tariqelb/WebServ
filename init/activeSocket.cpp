/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activeSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:31:49 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/06 20:29:13 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	activeReadSocket(struct webserv& web)
{
	int	i;
	int	j;
	int size;

	size = web.servers.size();
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			FD_SET(web.servers[i].socketFd[j], &web.reads);
			j++;
		}
		i++;
	}
}

void	activeWriteSocket(struct webserv& web)
{
	int	i;
	int	j;
	int size;

	size = web.servers.size();
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			FD_SET(web.servers[i].socketFd[j], &web.writes);
			j++;
		}
		i++;
	}
}

void	activeSocket(struct webserv& web)
{
	int	i;
	int	j;
	int	maxFd;
	int size;

	FD_ZERO(&web.reads);
	FD_ZERO(&web.writes);
	activeReadSocket(web);
	activeWriteSocket(web);
	i = 0;
	maxFd = -1;
	size = web.servers.size();
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			if (web.servers[i].socketFd[j] > maxFd)
				maxFd = web.servers[i].socketFd[j] ;
			j++;
		}
		i++;
	}
	web.maxReadFd = maxFd;
}
