/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 20:01:49 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/29 22:18:36 by tel-bouh         ###   ########.fr       */
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
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <sys/stat.h>

# define MAX_CONNECTION 5
# define MAX_PORT 3
# define HOST "localhost"
# define MAX_CLIENTS 10
# define CONTINUE "HTTP/1.0 100-continue"
# define CONFIGFILE	"./server.conf";
const char	ports[MAX_PORT][6] = {"8080", "8081", "8082"};


struct	location
{
	std::string											pattern;
	std::vector<std::string>							allow;//methods allowed in a location
	std::string											root;
	std::string											index;
	std::string											autoindex;
	std::vector<std::pair<std::string, std::string> >	error_page;
	std::string											upload;
	std::string											upload_store;
	std::string											cgi_ext;
	std::string											cgi_path;
};
// serverfile struct hold information from confige file each variable is hold one value or multiple if it std::string so its hold one value
// and if its std::vector so it hold mutiple value 
// the struct serverfile have internal vector the take a struct location in its template argument
// there some variable that have the same name in serverfile sturct and location struct and thats mean that the variable can
// being in server block and also in location block but the variable in location block override thet one in server block
// except for root varibale (directive) that if there is two root directive in server and location block the root in 
// location block is relative and must be add to the end of root variable in server block and that what i try to do 
// listen = the port(s) witch the server must be listen to
// server_name = the domain name(s) in that server
// error page = it consist of two value error code and default file , default file that needs to be serve is a particular error happen
// max body size = maximum number of byte that allowed to be uploded , only three unit i cover G M K (byte)
// root = the root director in witch you will be search to serve a particular request
// index = default file to serve
// location = block that holde muliple value
// pattern = uri for that loaction 
// allow = allowed methods in this loaction
// autoindex = hold on or off value , allow directory listen in a particular location
// upload = hold on or of value , allow upload of file in a particular location 
// cgi = extension of cgi file or request (i don't know till now) to be serve in a partocular location
// cgi_path = path to the file that will execute for a cgi
struct	serverfile
{
	std::vector<std::string>							listen;
	std::vector<std::string>							server_name;
	std::vector<std::pair<std::string, std::string> >	error_page;
	std::string											max_body_size;
	std::string											root;
	std::string											index;
	std::vector<struct location>						location;
};


// client struct the shall be hold the information about the request or responce 
// reqest and respoce is std::vector that take pair as its template arg so you can save key value from the reqest and responce
// addr is a sockaddr_storage struct the can serve ipv4 and ipv6 address but we can change it to ipv4 struct cause we just handle ipv4 connections
// len is the length of the addr struct a variable that we need it in a call of some functions
// fd is the file descriptor of the client returned from  accept function

struct client
{
	std::vector<std::pair<std::string, std::string> >	request;
	std::vector<std::pair<std::string, std::string> >	responce;
	struct sockaddr_storage								addr;
	socklen_t											len;
	int													fd;
};

struct server
{
	struct	addrinfo	*socket;
	int					socketFd;
	std::string			port;
};

//
struct	webserv
{
	struct addrinfo						hints;
	struct addrinfo						*server[MAX_PORT];
	std::vector<client>					clients;
	int									socketFd[MAX_PORT];
	int									status;
	fd_set								reads;
	fd_set								writes;
	fd_set								exceps;
	int									maxReadFd;
	int									maxWriteFd;
	fd_set								cReads;
	fd_set								tmp;
	std::vector<serverfile>				config;
};

// split.cpp // must be removed cause not allwed
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






// parse configue file functions

//parseConfigfun3.cpp // not used
int		checkForPairBrackets(std::vector<std::string>& file);
int		pairBrackets(std::vector<std::string> serv);

//checkForSemiColon.cpp  // not used
int		checkForSemiColon(std::vector<std::string> file);

//valideDirectiveName.cpp
std::string 	getDirectiveKey(std::string line);
std::string		getDirectiveValue(std::string line, std::string key);
int 			valideDirectiveValue(std::string line, std::string key);
int				valideDirectiveName(std::vector<std::string> file);

//valides.cpp
int		alphaDigit(char c);
int		getNbr(std::string line);
int		validePort(std::string line);
int		valideDomainName(std::string line);
int		valideFile(std::string line);
int		valideUnit(std::string line);
int		valideOnOff(std::string line);
int		valideExtension(std::string line);
int		valideScript(std::string line);
int		validePath(std::string line);




//parseConfigFile.cpp
int		parseConfigData(std::vector<std::string>& file);
void    parseConfigFile(struct webserv& web, int ac, char **av);

//getConfigData.cpp
void		getConfigData(struct webserv& web, std::vector<std::string> file);

//parseConfigFun1.cpp
int		isEmptyLines(std::string line);
int		isComment(std::string line);
int		isServerBlock(std::string line);
int		isLocationBlock(std::string line);
int		isBracket(std::string line);
int		endsWithSemiColon(std::string line);
void	countServerBlock(std::vector<std::string> file, int& nbr_of_server);
int		nbrOfLocBlock(std::vector<std::string> file);
int		closedBlock(std::vector<std::string> file);
void	removeEmptyLineAndComments(std::vector<std::string>& file);

//moveBracketsToNextLine.cpp
void	moveBracketsToNextLine(std::vector<std::string>& file);
int		isaBlockWithOpenBrackets(std::string& line);
int		isDirectiveWithCloseBrackets(std::string& line);

//splitSemicolons.cpp
void    removeDoubleSemiColons(std::string& line);
void	splitSemiColons(std::vector<std::string>& file);
int		isAloneSemiColon(std::string  line);
void    removeEmptySemiColons(std::vector<std::string>& file);

//valideServersBlock.cpp
int		valideServerName(std::string line);
int		isAclosedServer(std::vector<std::string> file, int line_index);
int		valideServersBlock(std::vector<std::string> file);

//checkConfigData.cpp
int	checkConfigData(struct webserv web);

// displayServerFile.cpp
void	displayServerFile(std::vector<struct serverfile> conf);



#endif
