/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlerequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:02:55 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/04/12 23:26:30 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

char    temp[2048] = "HTTP/1.0 200 OK\r\n Server: webserver-c\r\n Content-type: text/html\r\n\r\n <html>hello, world    </html>\r\n";



int	handleContinue(char	*line)
{
	std::string	str;
	int			rslt;

	str.assign(line);
	rslt = str.find("Expect: 100-continue");
	if (rslt != -1)
		return (0);
	return (1);
}

int	endOfTheRequest(std::string buffer)
{
	if (buffer.size() >= 4)
	{
		if (buffer.compare(buffer.size() - 4, 4, "\r\n\r\n") == 0)
			return (0);
	}
	return (1);
}

void	closeConnection(struct webserv& web, std::vector<client>::iterator& it, int client_i)
{
	close(web.clients[client_i].fd);
	FD_CLR(web.clients[client_i].fd , &web.cReads);
	web.clients.erase(it + client_i);
}


void	handleRequest(struct webserv& web)
{
	int								i;
	int								fd;
	int								rd;
	char							line[1000];
	std::stringstream				buffer;
	std::vector<client>::iterator	it;

	i = 0;
	while (i < web.clients.size())
	{
		it = web.clients.begin();
		fd = web.clients[i].fd;
		std::cout << "FD :" << fd << std::endl;
		if (FD_ISSET(fd, &web.cReads))
		{
			int	index = 0;
			while (1)
			{
				rd = 0;
				index++;
				memset(line, 0, 1000);
				rd = recv(web.clients[i].fd, line, 999, 0);
				std::cout << "recv " << rd << std::endl; 
				line[rd] = 0;
				if (rd > 0)
					buffer << line;
				if (rd == -1)
				{
					write(2, "Error in resv \n", 15);
					return;
				}
				if (handleContinue(line) == 0)
				{
					std::cout << CONTINUE << std::endl;
					send(web.clients[i].fd, CONTINUE, strlen(CONTINUE), 0);
				}
				else if (endOfTheRequest(buffer.str()) == 0)
					break;
				if (rd == 0)
				{
					std::cout << "*--------------------------*" << std::endl;
					std::cout << buffer << std::endl;
					std::cout << "*--------------------------*" << std::endl;
					break;
				}
				if (rd != 0 && rd != 999)
				{
					std::cout << "Get out" << std::endl;
					break;
				}
			}
			if (rd == 0)
			{
				printf("%s\n", "connection is ends\n");
				closeConnection(web, it, i);
			}
			else
			{
				printf("Clients request :\n");
				printf(" ----------------------------\n");
				printf("%s\n", (buffer.str()).c_str());
				printf(" ----------------------------\n");	
				parseRequest(web, web.clients[i], buffer);
				//getRequestData(web, buffer, fd);
				/*response(web, i);
				std::cout << "--------------------------------" << std::endl;
				std::cout << web.clients[i].rsp << std::endl;
				send(web.clients[i].fd, web.clients[i].rsp.c_str(), strlen(web.clients[i].rsp.c_str()), 0);
				std::cout << "--------------------------------" << std::endl;*/
				send(web.clients[i].fd, temp, strlen(temp), 0);
				closeConnection(web, it, i);
			}
		}
		i++;
	}
}
