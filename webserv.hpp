/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 20:01:49 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/11 15:29:24 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/errno.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <poll.h>
//# include <sys/epoll.h>
# include <sys/event.h>
# include <sys/time.h>
//# include <fnctl.h>

#include <vector>
#include <iostream>
#include <stdio.h>


# define PORT "8080"
# define HOST "localhost"
# define MAX_CLIENTS 10

struct client
{
	struct sockaddr_storage	client;
	int						fd;
	socklen_t				len;
};



struct	webserv
{
	struct addrinfo			hints;
	struct addrinfo			*server;
	struct client			clients[10];
	int						index;
	int						socketFd;
	int						status;
	fd_set					reads;
	fd_set					writes;
	fd_set					exceps;

	std::vector<std::pair<std::string, std::string> > request[100];
	int						reqIndex;
};

// split.cpp
char	**ft_split(char const *s, char c);
// splitSet.cpp
char	**ft_split_str(char *str, char *charset);
// parseRequest.cpp

void    getMethod(std::vector<std::pair<std::string, std::string> >& req, char *line);
void    getHeaders(std::vector<std::pair<std::string, std::string> >& req, char *line);
void    displyReqeust(std::vector<std::pair<std::string, std::string> > req);
void    getRequestInfo(struct webserv& web, char* buffer);


#endif
