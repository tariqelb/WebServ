/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequestData.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 18:20:46 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/16 18:43:55 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"



int	handleContinue(char	*line)
{
	std::string	str;
	int			rslt;

	str.assign(line);
	rslt = str.find("Expect: 100-continue");
	if (rslt != -1)
		return (0);
	return (1);
}

int	endOfTheRequest(std::string& buffer)
{
	if (buffer.size() >= 4)
	{
		if (buffer.compare(buffer.size() - 4, 4, "\r\n\r\n") == 0)
			return (0);
	}
	return (1);
}

void	closeConnection(struct webserv& web, std::vector<client>::iterator& it, int client_i)
{
	close(web.clients[client_i].fd);
	FD_CLR(web.clients[client_i].fd , &web.cReads);
	web.clients.erase(it + client_i);
}
