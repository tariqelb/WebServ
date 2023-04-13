/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 20:22:25 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/04/07 21:38:24 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

/*
void	getErrorPage(struct server serv, struct client clt, struct respReslt help)
{
	struct location	loc;
	std::string 	uri;
	int				i;
	int				size;
	std::string		error;

	uri = getRequestValue(clt, "URI");
	i = 0;
	size = serv.serverConfig.location.size();
	while (i < size)
	{
		if (serv.serverConfig.location[i].pattern == uri)
			loc = serv.serverConfig.location[i];
		i++;
	}
	if (loc.pattern.size() == 0)
	{
		if (serv.serverConfig.error_page.size() == 0)
			;//generateErrorPage(help.error);
		else
		{
			i = 0;
			size = serv.serverConfig.error_page.size();
			while (i < size)
			{
				if (serv.serverConfig.error_page[i].first = help.error)
					i++;
			}
		}
	}
}

std::string	getRequestValue(struct client clt, std::string key)
{
	int 		i;
	int 		size;
	std::string rtrn;

	i = 0;
	size = clt.request.size();
	while (i < size)
	{
		if (clt.request[i].first == key)
			return (clt.request[i].second);
		i++;
	}
	return (rtrn);
}	

struct server	getServerForResponse(struct webserv web, std::string port)
{
	int 			i;
	int 			j;
	int 			size;
	int				listen_size;;
	struct server	serv;
	
	i = 0;
	size = web.servers.size();
	while (i < size)
	{
		j = 0;
		listen_size = web.servers[i].serverConfig.listen.size();
		while (j < listen_size)
		{
			if (web.servers[i].serverConfig.listen[j] == port)
				return (web.servers[i]);
			j++;
		}
		i++;
	}
	return (serv);
}

void	getResponse(struct webserv web, struct client& clt)
{
	std::cout << "get response fun" << std::endl;
	struct server serv;
	std::string port;
	struct respReslt help;
	
	help.error = 0;
	help.autoindex = 0;
	port = getRequestValue(clt, "Port");
	serv = getServerForResponse(web, port);
	responseToRequest(serv, clt, help);
	if (help.error)
		std::cout << "Error : " << help.error << std::endl;//getStatusCode();
	else
	{
		char			buff[10000];
		std::string		name;
		std::ifstream	content;

		if (help.root[0] != '.')
		{
			help.root = "." + help.root;
		}
		if (help.root[help.root.size() - 1] != '/')
		{
			name = help.root + "/";
			name = name + help.index;
		}
		else
			name = name + help.index;
		content.open(name.c_str());
		if (content.is_open() == false)
		{
			std::cout << "Can not open file " << name << std::endl;
			help.error = 404;
		}
		else
		{
			clt.rsp += "HTTP/1.0 200 OK\r\nServer: webserver-c\r\nContent-type: text/html\r\n\r\n";
			std::string	line;
			while (getline(content, line))
			{
				line += "\n";
				clt.rsp += line;
			}
			clt.rsp += "\r\n";
			content.close();
		}
	}
}


void	response(struct webserv& web, int index)
{
	int i;
	int size;
	
	i = 0;
	size = web.clients[index].request.size();
	while (i < size)
	{
		
		if (web.clients[index].request[i].first == "Method")
		{
			if (web.clients[index].request[i].second == "GET")
				getResponse(web, web.clients[i]);
			//if (web.clients[index].request[i].second == "Post")
			//	postResponse();
			//if (web.clients[index].request[i].second == "DELETE")
			//	deleteResponse();
		}
		i++;
	}
}*/
