/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 19:47:52 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/04/01 00:24:30 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


void	getServerConfig(struct webserv& web)
{
	int i;
	int size;

	i = 0;
	size = web.config.size();
	while (i < size)
	{
		struct server	serv;
		
		serv.serverConfig = web.config[i];
		web.servers.push_back(serv);
		int j = 0;
		while (j < serv.serverConfig.listen.size())
		{
			struct addrinfo	*info;
			web.servers[i].socket.push_back(info);
			j++;
		}
		i++;
	}
}

void	initWebStrcut(struct webserv& web)
{
	int		i;
	int		j;
	int 	len;
	int		flag;
	int		size;

	flag = 0;
	getServerConfig(web);
	size = web.servers.size();
	memset(&web.hints, 0, sizeof(web.hints));
	web.hints.ai_family = AF_INET;
	web.hints.ai_socktype = SOCK_STREAM;
	web.hints.ai_flags = AI_PASSIVE;
	web.hints.ai_protocol = IPPROTO_TCP;
	i = 0;
	while (i < size)
	{
		j = 0;
		len = web.servers[i].serverConfig.listen.size();
		while (j < len)
		{
			char *port = const_cast<char*>( web.servers[i].serverConfig.listen[j].c_str());
			web.status = 0;
			web.status = getaddrinfo(HOST, port, &web.hints, &web.servers[i].socket[j]); 
			if (web.status != 0)
			{
				std::cerr << "error : init web struct in port ";
				std::cout << web.servers[i].serverConfig.listen[j] << std::endl;
			}
			else
				flag++;
			j++;
		}
		i++;
	}

	if (flag == 0)
		web.status = 1;
	else
		web.status = 0;
}
/*
void	freedWeb(struct webserv& web)
{
	int	i;

	i = 0;
	while (i < MAX_PORT)
	{
		close(web.socketFd[i]);
		freeaddrinfo(web.server[i]);
		i++;
	}
}*/

int	main(int ac, char **av)
{
	struct	webserv		web;
	struct	timeval		tv;
	int					index;
	int					flag;

	index = 0;
	tv.tv_sec = 120;
	tv.tv_usec = 0;
	//get data from config file
	parseConfigFile(web, ac, av);
	if (web.status != 0)
	{
		std::cerr << "No valid server found" << std::endl;
		return (1);
	}
	//Fill the struct
	initWebStrcut(web);
	if (web.status != 0)
	{
		write(2, "Error : init webserv struct\n", 29);
		return (1);
	}
	//Display IP and PORT
	displayHostPort(web);
	/*flag = initServer(web);
	if (flag)
	{
		write(2, "Error : initialing webserv\n", 27);
		freedWeb(web);
		return (1);
	}
	//set select for incomming connections
	activeSocket(web);
	while (1)
	{
		web.status = -1;
		web.tmp = web.reads;
		printf("Wait in select:\n");
		web.status = select(web.maxReadFd + 1, &web.tmp, &web.writes, &web.exceps, &tv);
		if (web.status == 0)
		{
			write(2, "server time listening out\n", 26);
			return (1);
		}
		if (web.status < 0)
		{
			write(2, "Error : Fail occur on select function\n", 39); // perror
			freedWeb(web);
			return (1);

		}
		else //(web.status == 0)
			handleConnection(web);
		handleRequest(web);
	}
	freedWeb(web);*/
	
	return (0);
}
