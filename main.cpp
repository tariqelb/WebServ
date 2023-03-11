/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 19:47:52 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/11 17:41:43 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


char	temp[2048] = "HTTP/1.0 200 OK\r\n Server: webserver-c\r\n Content-type: text/html\r\n\r\n <html>hello, world</html>\r\n";

void	initWebStrcut(struct webserv& web)
{
	memset(&web.hints, 0, sizeof(web.hints));
	web.hints.ai_family = AF_INET;
	web.hints.ai_socktype = SOCK_STREAM;
	web.hints.ai_flags = AI_PASSIVE;
	web.hints.ai_protocol = IPPROTO_TCP;
	web.status = getaddrinfo(HOST, PORT, &web.hints, &web.server);
	web.reqIndex = 0;
	web.index = 0;
}
void	displayHostPort(struct webserv& web)
{
	struct addrinfo	*p;
	char			str[32];
	void			*addr;

	p = web.server;
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
}

void	freedWeb(struct webserv& web)
{
	close(web.socketFd);
	freeaddrinfo(web.server);
}


void	readif(struct webserv& web, int index);
void	handleConnection(struct webserv& web)
{
		int		i;
		int		fd;
		int		rd;
		char 	*buffer;

		if (FD_ISSET(web.socketFd, &web.reads))
		{
				web.clients[web.index].len = sizeof(web.clients[web.index].client);
				web.clients[web.index].fd = accept(web.socketFd, (struct sockaddr *)&web.clients[web.index].client, &web.clients[web.index].len);
				printf("accepted %d\n", web.clients[web.index].fd);
				if (web.clients[web.index].fd < 0)
				{
					write(2, "Error : Fail connecting to an client\n", 37);
					return ;
				}
				else
				{
					FD_SET(web.clients[web.index].fd, &web.reads);
					web.index++;
				}
		}	
		printf("pass : %d   :  %d\n", web.clients[web.index - 1].fd, web.index);
		i = 0;
		while (i < web.index)
		{
			printf("i : %d\n", i);
			fd = web.clients[i].fd;
			if (FD_ISSET(fd, &web.reads))
			{

				printf("start\n");
				buffer	= (char *) malloc(2048);
				rd = recv(web.clients[i].fd, buffer, 2048, MSG_PEEK);
				printf("middle\n");
				if (rd == 0)
				{
					printf("%s\n", "connection is ends\n");
					close(web.clients[i].fd);
					FD_CLR(web.clients[i].fd , &web.reads);
					web.index--;
				}
				else
				{
					int rt = 0;
					printf("Clients request statrt here :\n -------------------------------------------\n");
					rt = printf("[%s\n]", buffer);
					printf(" -------------------------------------------\nThe size is : %d\n", rt);
					//getRequestInfo(web, buffer);
					if (buffer != NULL)
						free(buffer);
					memset(buffer, 0, 2048);
					send(web.clients[i].fd, temp, strlen(temp), 0);
					FD_CLR(web.clients[i].fd , &web.reads);
					printf("accepted send %d\n", web.clients[i].fd);
				}
				printf("Done\n");
			}
			i++;
		}
}


int	main(int ac, char **av)
{
	struct webserv	web;
	struct timeval	tv;
	int				index;

	index = 0;
	tv.tv_sec = 120;
	tv.tv_usec = 0;
	//Fill the struct
	initWebStrcut(web);
	if (web.status != 0)
	{
		write(2, "Error : init webserv struct\n", 29);
		return (1);
	}
	//Display IP and PORT
	displayHostPort(web);
	//create socket listening to localhost in ipv4 and ipv6 
	web.socketFd = socket(web.server->ai_family, web.server->ai_socktype, web.server->ai_protocol);
	//printf("fd : %d\nfd : %d\n", web.socketFd[0], web.socketFd[1]);
	if (web.socketFd < 0)
	{
		close(web.socketFd);
		write(2, "Error : create webserv socket\n", 31);
		return (1);
	}
	//Bind socket to localhost address stored in web.server for both version 
	web.status = bind(web.socketFd, web.server->ai_addr, web.server->ai_addrlen);
	printf("status in bind %d : Errno : %d\n", web.status, errno);
	if (web.status != 0)
	{
		write(2, "Error : Fail to bind socket to localhost\n", 42);
		freedWeb(web);
		return (1);
	}
	//Turn sockets to passive mode (listen to incamming connection)
	web.status = listen(web.socketFd, 5);
	if (web.status != 0)
	{	
		write(2, "Error : Fail to make socket listening to connection\n", 53);
		freedWeb(web);
		return (1);
	}
	//set select for incomming connections
	FD_ZERO(&web.reads);
	FD_ZERO(&web.writes);
	FD_ZERO(&web.exceps);
	FD_SET(web.socketFd, &web.reads);
	//web.max_socket = web.socketFd;
	while (1)
	{
		fd_set	newConn;
		newConn = web.reads;
		web.status = -1;
		printf("Wait in select:\n");
		web.status = select(MAX_CLIENTS + 1, &web.reads, &web.writes, &web.exceps, &tv);
		if (web.status == 0)
		{
			write(2, "server time listening out\n", 26);
			return (1);
		}
		if (web.status < 0)
		{
			write(2, "Error : Fail occur on select function\n", 39);
			freedWeb(web);
			return (1);

		}
		else //(web.status == 0)
			handleConnection(web);
	}
	freedWeb(web);
	return (0);
}
