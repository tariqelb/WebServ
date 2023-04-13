/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   responseToRequest.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 20:10:17 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/04/06 23:45:05 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void responseToRequest(struct server serv, struct client clt, struct respReslt& help)
{
	int				i;
	int				size;
	int				allow_method;
	struct location	loc;
	std::string		uri;
	std::string		method;

	uri = getRequestValue(clt, "URI");
	i = 0;
	size = serv.serverConfig.location.size();
	while (i < size)
	{
		if (serv.serverConfig.location[i].pattern == uri)
			loc = serv.serverConfig.location[i];
		i++;
	}
	if (loc.pattern.size() == 0)
	{
		if (uri == "/")
		{
			if (serv.serverConfig.root.size() == 0)
			{
				help.error = 500;
				return ;
			}
			if (serv.serverConfig.index.size() == 0)
			{
				help.error = 404;
				return ;
			}
			help.root = serv.serverConfig.root;
			help.index = serv.serverConfig.index;
		}
		else
		{
			help.error = 404;
			return ;
		}
	}
	else
	{
		method = getRequestValue(clt, "Method");
		i = 0;
		allow_method = 0;
		size = loc.allow.size();
		while (i < size)
		{
			if (method == loc.allow[i])
				allow_method = 1;
			i++;
		}
		if (allow_method == 0)
		{
			help.error = 405;
			return ;
		}
		help.root = loc.root;
		if (help.root.size() == 0)
			help.root = serv.serverConfig.root;
		help.index = loc.index;
		if (help.index.size() == 0)
			help.index = serv.serverConfig.index;
		if (help.root.size() == 0)
		{
			help.error = 500;
			return ;
		}
		if (help.index.size() == 0 && loc.autoindex != "on")
		{
			help.error = 404;
			return ;
		}
		if (help.index.size() == 0 && loc.autoindex == "on")
			help.autoindex = 1;
	}

	std::cout << "Reach here" << std::endl;
	std::cout << "Index : " << help.index << std::endl;
	std::cout << "Root : " << help.root << std::endl;
	std::cout << "Autoindex : " << help.autoindex << std::endl;

}
