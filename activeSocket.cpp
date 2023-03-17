/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   activeSocket.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 16:31:49 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/16 19:37:23 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	activeReadSocket(struct webserv& web)
{
	int	i;

	i = 0;
	while (i < MAX_PORT)
	{
		FD_SET(web.socketFd[i], &web.reads);
		i++;
	}
}

void	activeWriteSocket(struct webserv& web)
{
	int	i;

	i = 0;
	while (i < MAX_PORT)
	{
		FD_SET(web.socketFd[i], &web.writes);
		i++;
	}
}

void	activeExceptSocket(struct webserv& web)
{
	int	i;

	i = 0;
	while (i < MAX_PORT)
	{
		FD_SET(web.socketFd[i], &web.exceps);
		i++;
	}
}

void	activeSocket(struct webserv& web)
{
	int	i;
	int	maxFd;

	FD_ZERO(&web.reads);
	FD_ZERO(&web.writes);
	FD_ZERO(&web.exceps);
	activeReadSocket(web);
	i = 0;
	maxFd = -1;
	while (i < MAX_PORT)
	{
		if (web.socketFd[i] > maxFd)
			maxFd = web.socketFd[i];
		i++;
	}
	web.maxReadFd = maxFd;
}
