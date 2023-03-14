/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 19:47:52 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/14 15:24:33 by tel-bouh         ###   ########.fr       */
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

void	handleConnection(struct webserv& web)
{
	struct client newClient;
	printf("HC--->");

	if (FD_ISSET(web.socketFd, &web.reads))
	{
			newClient.len = sizeof(newClient.addr);
			newClient.fd = accept(web.socketFd, (struct sockaddr *)&newClient.addr, &newClient.len);
			printf("New connection accepted with fd :  %d\n", newClient.fd);
			if (newClient.fd < 0)
			{
				write(2, "Error : Fail connecting to an client\n", 37);
				return ;
			}
			else
			{
				FD_SET(newClient.fd, &web.reads);
				web.clients.push_back(newClient);
			}
	}	
}

void	handlerequest(struct webserv& web)
{
	int								i;
	int								fd;
	int								rd;
	char 							buffer[2050];
	char							line[100];
	std::vector<client>::iterator	it;

	i = 0;
	while (i < web.clients.size())
	{
		it = web.clients.begin();
		memset(buffer, 0, 2050);
		fd = web.clients[i].fd;
		if (FD_ISSET(fd, &web.reads))
		{
			printf("start\n");
			int	index = 0;
			while (1)
			{
				index++;
				rd = recv(web.clients[i].fd, line, 99, 0);
				line[rd] = 0;
				if (rd > 0)
					strcat(buffer, line);
				if (rd == -1)
				{
					printf("errno : %d\n", errno);
					write(2, "Error in resv \n", 15);
					return;
				}
				if (buffer[strlen(buffer) - 1] == '\n' && buffer[strlen(buffer) - 2] == '\r')
				{
					printf("yes\n");
					break;
				}
			}
			if (rd == 0)
			{
				printf("%s\n", "connection is ends\n");
				close(web.clients[i].fd);
				FD_CLR(web.clients[i].fd , &web.reads); // makhdamx
				web.clients.erase(it + i);
			}
			else
			{
				int rt = 0;
				printf("Clients request statrt here :\n -------------------------------------------\n");
				rt = printf("[\n%s\n]\n", buffer);printf(" -------------------------------------------\n");
				getRequestInfo(web.clients, buffer);
				int snd = send(web.clients[i].fd, temp, strlen(temp), 0);
				printf("send %d\n", snd);
				close(web.clients[i].fd);
				FD_CLR(web.clients[i].fd , &web.reads);
				web.clients.erase(it + i);
				printf("accepted send and finish %d\n", web.clients[i].fd);
			}
			printf("Done\n");
		}
		i++;
	}
}

int	initServer(struct webserv& web)
{
	struct addrinfo	*ptr;
	int				valide;
	int				status;
	int 			reuse;
	
	reuse = 1;
	status = 0;
	ptr = web.server;
	while (ptr != NULL)
	{
		valide = 0;
		//create socket listening to localhost in ipv4 and ipv6 
		web.socketFd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (web.socketFd < 0)
			close(web.socketFd);
		else
		{
			valide++;
			//set file descriptor of the socket to non-blocking
			int flags = fcntl(web.socketFd, F_GETFL, 0);
			//printf("stt : %d %d %d\n", flags, O_NONBLOCK, (flags & O_NONBLOCK) == O_NONBLOCK);
			errno = 0;
			status = fcntl(web.socketFd, F_SETFL, flags & O_NONBLOCK);
			if (status < 0)
				close(web.socketFd);
			else
			{
			//	printf("stt : %d %d %d %d\n", valide, status, O_NONBLOCK, (status & O_NONBLOCK) == O_NONBLOCK);
				valide++;
				//set bind to reuse the some local address even if she is in Time Wait mode
				status = setsockopt(web.socketFd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
				if (status < 0)
					close(web.socketFd);
				else
				{
					valide++;
					//Bind socket to localhost address stored in web.server for both version 
					status = bind(web.socketFd, ptr->ai_addr, ptr->ai_addrlen);
					if (status < 0)
						close(web.socketFd);
					else
					{
						valide++;
						//Turn sockets to passive mode (listen to incamming connection) max queue 5
						status = listen(web.socketFd, 5);
						if (status < 0)
							close(web.socketFd);
						else
							valide++;
					}
				}
			}
		}
		if (valide == 5)
			return (0);
		ptr = ptr->ai_next;
	}
	return (1);
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
	//Fill the struct
	initWebStrcut(web);
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
	FD_ZERO(&web.reads);
	FD_ZERO(&web.writes);
	FD_ZERO(&web.exceps);
	FD_SET(web.socketFd, &web.reads);
	while (1)
	{
		web.status = -1;
		printf("Wait in select:\n");
		web.status = select(web.socketFd + 1, &web.reads, &web.writes, &web.exceps, &tv);
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
		handlerequest(web);
	}
	freedWeb(web);
	return (0);
}
