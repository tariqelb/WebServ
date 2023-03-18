/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 20:01:49 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/18 15:33:30 by tel-bouh         ###   ########.fr       */
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
# include <fcntl.h>

#include <vector>
#include <iostream>
#include <stdio.h>


# define MAX_CONNECTION 5
# define MAX_PORT 3
# define HOST "localhost"
# define MAX_CLIENTS 10
# define CONTINUE "HTTP/1.0 100-continue"
# define CONFIGFILE	"./server.conf";
const char	ports[MAX_PORT][6] = {"8080", "8081", "8082"};


struct	location
{
	std::string		location;
	std::string		allow[3];//methods allowed in a location
	std::string		root;
	std::string		index_file;
	std::string		autoindex;
	std::string		limit_except[3];
	std::string		error_page[2];
	std::string		upload;
	std::string		upload_store;
	std::string		cgi_ext;
	std::string		cgi_path;
};

struct	serverfile
{
	std::string						listen;
	std::string						server_name;
	std::string						error_page[2];
	std::string						max_body_size;
	std::string						root;
	std::string						index;
	std::vector<struct location>	location;
};




struct client
{
	std::vector<std::pair<std::string, std::string> >	request;
	std::vector<std::pair<std::string, std::string> >	responce;
	struct sockaddr_storage								addr;
	socklen_t											len;
	int													fd;
};


struct	webserv
{
	struct addrinfo			hints;
	struct addrinfo			*server[MAX_PORT];
	std::vector<client>		clients;
	int						socketFd[MAX_PORT];
	int						status;
	fd_set					reads;
	fd_set					writes;
	fd_set					exceps;
	int						maxReadFd;
	int						maxWriteFd;
	fd_set					cReads;
	fd_set					tmp;
	struct serverfile		config;
};

// split.cpp
char	**ft_split(char const *s, char c);

// parseRequest.cpp
void    getMethod(std::vector<std::pair<std::string, std::string> >& req, char *line);
void    getHeaders(std::vector<std::pair<std::string, std::string> >& req, char *line);
void    getRequestInfo(std::vector<client> client, char* buffer);
void    displyReqeust(std::vector<std::pair<std::string, std::string> > req);

//parseRequestData.cpp
int		handleContinue(char *line); 
int		endOfTheRequest(std::string& buffer);
void    closeConnection(struct webserv& web, std::vector<client>::iterator& it, int client_i);

//displayHostPort.cpp
void    displayHostPort(struct webserv& web);

//handleconnection.cpp
void    handleConnection(struct webserv& web);

//initServer.cpp
int 	initServer(struct webserv& web);

//handleRequest.cpp
void    handleRequest(struct webserv& web);

//activeSocket.cpp
void	activeReadSocket(struct webserv& web);
void	activeWriteSocket(struct webserv& web);
void	activeExceptSocket(struct webserv& web);
void	activeSocket(struct webserv& web);


//parseConfigFile.cpp


void    parseConfigFile(struct webserv& web, int ac, char **av);


#endif
