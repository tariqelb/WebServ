/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getRequestData.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 22:26:45 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/04/07 21:01:52 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"





/*
void	getRequestData(struct webserv& web, std::string buff, int fd)
{
	std::pair<std::string, std::string> pr1;
	std::pair<std::string, std::string> pr2;
	std::pair<std::string, std::string> pr3;
	const char *host = "127.0.0.1";
	const char *http = "HTTP/1.1";

	pr1.first = "Method";
	pr1.second = buff.substr(0, buff.find(" ", 0)); //"GET";
	pr2.first = "URI";
	pr2.second = buff.substr(4, buff.find(http, 0) - 5);
	pr3.first = "Port";
	pr3.second = buff.substr(buff.find(host, 0) + 10, 4);
	
	int i = 0;
	int size = web.clients.size();
	int j = 0;
	while (i < size)
	{
		if (web.clients[i].fd == fd)
		{
			web.clients[i].request.push_back(pr1);
			web.clients[i].request.push_back(pr2);
			web.clients[i].request.push_back(pr3);
			j = i;
		}
		i++;
	}
	std::cout << web.clients[j].request[0].first << " : ";
	std::cout << web.clients[j].request[0].second << std::endl;
	std::cout << web.clients[j].request[1].first << " : ";
	std::cout << web.clients[j].request[1].second << std::endl;
	std::cout << web.clients[j].request[2].first << " : ";
	std::cout << web.clients[j].request[2].second << std::endl;

}*/
