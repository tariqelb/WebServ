/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   displayHostPort.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:01:29 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/16 14:31:40 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	displayHostPort(struct webserv& web)
{
	struct addrinfo	*p;
	char			str[32];
	void			*addr;
	int				i;

	i = 0;
	while (i < MAX_PORT)
	{
		p = web.server[i];
		while (p != NULL)
		{
			if (p->ai_family == AF_INET)
			{
				addr = &((struct sockaddr_in *) p->ai_addr)->sin_addr;
				inet_ntop(p->ai_family, addr, str, sizeof(str));
				printf("HostIP4 : %s\nPost : %hu\n", str, ntohs(((struct sockaddr_in *) p->ai_addr)->sin_port));
			}
			else if (p->ai_family == AF_INET6)
			{
				addr = &((struct sockaddr_in6 *) p->ai_addr)->sin6_addr;
				inet_ntop(p->ai_family, addr, str, sizeof(str));
				printf("HostIP6 : %s\nPost : %hu\n", str, ntohs(((sockaddr_in6 *) p->ai_addr)->sin6_port));
			}
			p = p->ai_next;
		}
		memset(str, 0, 32);
		memset(str, 0, sizeof(addr));
		i++;
	}
}
