/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 19:47:52 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/04/03 22:15:36 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"



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
		std::cerr << "Error : init webserv struct" << std::endl;
		return (1);
	}
	//Display IP and PORT
	displayHostPort(web);
	flag = initServer(web);
	if (flag)
	{
		std::cerr << "Error : initialing webserv" << std::endl;
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
			std::cerr << "server time listening out" << std::endl;
			freedWeb(web);
			return (1);
		}
		if (web.status < 0)
		{
			std::cerr << "Error : Fail occur on select function" << std::endl; 
			freedWeb(web);
			return (1);

		}
		else //(web.status == 0)
			handleConnection(web);
		handleRequest(web);
	}
	
	freedWeb(web);
	
	return (0);
}
