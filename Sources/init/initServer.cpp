/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:12:36 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/07/30 11:29:17 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

int	initServer(struct webserv& web)
{
	struct addrinfo								*ptr;
	int											valide;
	int											status;
	int 										reuse;
	int											i;
	int											j;
	int											size;
	int											len;
	int											connected_socket;
	int											backlog;

	reuse = 1;//set address to be reuse 
	status = 0;
	i = 0;
	connected_socket = 0;
	size = web.servers.size(); 
	while (i < size)
	{

		j = 0;
		len = web.servers[i].socket.size();
		while (j < len)
		{
			ptr = web.servers[i].socket[j];
			while (ptr != NULL)
			{
				valide = 0;
				//create socket listening to localhost in ipv4
				web.servers[i].socketFd[j] = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (web.servers[i].socketFd[j] > 0)
				{
					valide++;
					
					int on = 1;				
					reuse = setsockopt(web.servers[i].socketFd[j], SOL_SOCKET, SO_NOSIGPIPE, (char *)&on, sizeof(on));
					if (reuse < 0)
						close(web.servers[i].socketFd[j]);
					else
					{	
						valide++;
						//set file descriptor of the socket to non-blocking
						int flags = fcntl(web.servers[i].socketFd[j], F_GETFL, 0);
						flags |= O_NONBLOCK;//!
						//printf("stt : %d %d %d\n", flags, O_NONBLOCK, (flags & O_NONBLOCK) == O_NONBLOCK);
						status = fcntl(web.servers[i].socketFd[j], F_SETFL, flags);
						if (status < 0)
							close(web.servers[i].socketFd[j]);
						else
						{
						//	printf("stt : %d %d %d %d\n", valide, status, O_NONBLOCK, (status & O_NONBLOCK) == O_NONBLOCK);
							valide++;
							//set bind to reuse the some local address even if she is in Time Wait mode
							status = setsockopt(web.servers[i].socketFd[j], SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
							if (status < 0)
								close(web.servers[i].socketFd[j]);
							else
							{
								valide++;
								//Bind socket to localhost address stored in web.server 
								status = bind(web.servers[i].socketFd[j], ptr->ai_addr, ptr->ai_addrlen);
								if (status < 0)
								{
									close(web.servers[i].socketFd[j]);
								}
								else
								{
									valide++;
									backlog = SOMAXCONN;
									//Turn sockets to passive mode (listen to incamming connection) max queue 5
									status = listen(web.servers[i].socketFd[j], backlog);
									if (status < 0)
										close(web.servers[i].socketFd[j]);
									else
										valide++;
								}
							}
						}
					}
				}
				if (valide == 6)
				{
					// std::cout << "listen to port number ";
					// std::cout << web.servers[i].serverConfig.listen[j] << " is established" << std::endl;
					connected_socket++;
					while (ptr)
						ptr = ptr->ai_next;
				}
				else
				{
					ptr = ptr->ai_next;
				}
			}
			if (valide != 6)
			{
				web.servers[i].socketFd[j] = -1;
			}
			j++;
		}
		i++;
	}
	std::vector<int>::iterator					fd_it;
	std::vector<struct addrinfo *>::iterator	sock_it;
	i = 0;
	size = web.servers.size(); 
	while (i < size)
	{
		j = 0;
		len = web.servers[i].socket.size();
		sock_it = web.servers[i].socket.begin();
		fd_it = web.servers[i].socketFd.begin();
		while (j < len)
		{
			if (web.servers[i].socketFd[j] == -1)
			{
				web.servers[i].socket.erase(sock_it + j);
				web.servers[i].socketFd.erase(fd_it + j);
				sock_it = web.servers[i].socket.begin();
				fd_it = web.servers[i].socketFd.begin();
				len = web.servers[i].socket.size();
				j = -1;
			}
			j++;
		}
		i++;
	}
	/*std::cout << "-------------------------\n";
	i = 0;
	size = web.servers.size(); 
	while (i < size)
	{
		j = 0;
		len = web.servers[i].socket.size();
		while (j < len)
		{
			std::cout << "Fd af: " << web.servers[i].socketFd[j] << std::endl;
			j++;
		}
		i++;
	}*/
	web.nbr_of_connections = connected_socket;
	if (connected_socket)
		return (0);
	return (1);
}