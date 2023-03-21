/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 19:47:52 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/21 13:36:57 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


void	initWebStrcut(struct webserv& web)
{
	int		i;
	int		flag = MAX_PORT;

	memset(&web.hints, 0, sizeof(web.hints));
	web.hints.ai_family = AF_INET;
	web.hints.ai_socktype = SOCK_STREAM;
	web.hints.ai_flags = AI_PASSIVE;
	web.hints.ai_protocol = IPPROTO_TCP;
	i = 0;
	while (i < MAX_PORT)
	{
		web.status = 0;
		web.status = getaddrinfo(HOST, ports[i], &web.hints, &web.server[i]); 
		if (web.status != 0)
		{
			flag--;
			write(2, "error : init web struct in port ", 32);
			write(2, ports[i], strlen(ports[i]));
			write(2, "\n", 1);
		}
		i++;
	}
	if (flag == 0)
		web.status = 1;
	else
		web.status = 0;
}

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
}

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
		write(2, "Error in config file\n", 21);
		return (1);
	}
	//Fill the struct
	/*initWebStrcut(web);
	if (web.status != 0)
	{
		write(2, "Error : init webserv struct\n", 29);
		return (1);
	}
	//Display IP and PORT
	displayHostPort(web);
	flag = initServer(web);
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
