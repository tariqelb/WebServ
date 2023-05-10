/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlerequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:02:55 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/10 22:05:44 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

char    temp[2048] = "HTTP/1.0 200 OK\r\n Server: webserver-c\r\n Content-type: text/html\r\n\r\n <html>hello, world    </html>\r\n";



int	toInt(std::string nbr)
{
	int i;
	int rst;
	int size;
	int	sign;

	i = 0;
	rst = 0;
	sign = 1;
	size = nbr.size();
	while (i < size)
	{
		rst = rst * 10;
		rst = rst + (nbr[i] - 48);
		i++;
	}
	return (rst * sign);
}



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

int	contentLength(std::string	buffer, unsigned long& content_len)
{
	int i;
	int j;
	int size;
	int find;
	
	size = buffer.size();
	if (content_len == 0)
	{
		find = buffer.find("Content-Length:");
		if (find > 0)
		{
			i = find + 15;
			while (i < size && buffer[i] == ' ')
				i++;
			j = 0;
			while (i + j < size && (buffer[i + j] >= '0' && buffer[i + j] <= '9'))
				j++;
			content_len = toInt(buffer.substr(i, j));
		}
		else
			content_len = -1;
	}
	if (content_len > 0)
	{
		find = buffer.find("\r\n\r\n") + 4;
		if (find > 0)
		{
			std::cout << "out " << buffer.size() << " " << (content_len + find) << std::endl;
			if (buffer.size() >= (content_len + find))
				return (1);
		}
	}
	return (0);
}

int		getBoundary(std::string buffer, std::string& boundary)
{
	int				i;
	int				size;
	int 			find;
	std::string		temp;

	find = buffer.find("Content-Type:");
	if (find == -1)
		return (-1);
	i = 0;
	size = buffer.size();
	while (find + i < size && buffer[find + i] != '\n')
		i++;
	temp.assign(buffer.substr(find, i));
	find = temp.find("boundary=");
	if (find == -1)
		return (-1);
	i = 0;
	find = find + 9;
	size = temp.size();
	while (find + i < size && (temp[find + i] != '\n' && temp[find + i] != ' ' && temp[find + i] != '\t'))
		i++;
	boundary = temp.substr(find, i - 1);
	boundary += "--";
	return (0);
}

void	getBodyType(std::string buffer, struct body& bodys)
{
	int				i;
	int				j;
	int				size;
	int				find;
	std::string		temp;

	find = buffer.find("Content-Type:");
	if (find >= 0)
	{
		size = buffer.size();
		while (find + i < size && buffer[find + i] != '\n')
			i++;
		temp.assign(buffer.substr(find, i));
		find = temp.find("boundary=");
		if (find >= 0)
		{
			i = 0;
			find = find + 9;
			size = temp.size();
			while (find + i < size && (temp[find + i] != '\n' && temp[find + i] != ' ' && temp[find + i] != '\t'))
				i++;
			bodys.boundary = temp.substr(find, i - 1);
			bodys.boundary += "--";
			bodys.boundary_flag = 1;
			return;
		}
	}
	find = buffer.find("Transfer-Encouding:");
	if (find >= 0)
	{
		i = find + 19;
		j = 0;
		while (i + j < size && buffer[i + j] != '\n')
			j++;
		temp = buffer.substr(i, j);
		find = temp.find("chunked");
		if (find >= 0)
		{
			find = buffer.find("Content-Length:");
			if (find >= 0)
			{
				size = buffer.size();
				i = find + 15;
				while (i < size && buffer[i] == ' ')
					i++;
				j = 0;
				while (i + j < size && (buffer[i + j] >= '0' && buffer[i + j] <= '9'))
					j++;
				bodys.chunks_len = toInt(buffer.substr(i, j));
				if (bodys.chunks_len > 0)
				{
					bodys.chunks_flag = 1;
					return;
				}
			}
		}
	}
	find = buffer.find("Content-Length:");
	if (find >= 0)
	{
		size = buffer.size();
		i = find + 15;
		while (i < size && buffer[i] == ' ')
			i++;
		j = 0;
		while (i + j < size && (buffer[i + j] >= '0' && buffer[i + j] <= '9'))
			j++;
		bodys.content_len = toInt(buffer.substr(i, j));
		std::cout << "get to here pls inform me fkmndfkln\n";
		std::cout << "content length  ========================= > "  <<  bodys.content_len << std::endl;
		if (bodys.content_len > 0)
			bodys.content_length_flag = 1;
		else
			bodys.cr_nl_flag = 1;	
	}
}


int	endOfTheRequest(std::string buffer, struct body& bodys, int& flag , std::string& boundary, unsigned long& content_len)
{
	int find;
	int i;
	int len;
	std::cout << "_______________________________________________________________________________________ end" << std::endl;
	getBodyType(buffer, bodys);
	/*if (bodys.boundary_flag)
		std::cout << "1 " << bodys.boundary << std::endl;
	if (bodys.chunks_flag)
		std::cout << "2 " << bodys.chunks_len << std::endl;
	if (bodys.content_length_flag)
		std::cout << "3 " << bodys.content_len << std::endl;
	if (bodys.cr_nl_flag)
		std::cout << "4 " << bodys.cr_nl_flag << std::endl;
	
	if (bodys.boundary_flag)
	{
		if (buffer.find(boundary) != -1)
			return (0);
	}
	if (bodys.chunks_flag)
	{
		;
	}
	if (bodys.content_length_flag)
	{
		find = buffer.find("\r\n\r\n") + 4;
		if (buffer.size() >= (bodys.content_len + find))
				return (0);
	}
	if (bodys.cr_nl_flag)
	{
		if (buffer.compare(buffer.size() - 4, 4, "\r\n\r\n") == 0)
		{
			std::cout << "rrrsss\n" ;
			return (0);
		}
	}*/	
	if (boundary.size() == 0)
	{
		find = getBoundary(buffer, boundary);
		if (find == -1)
			flag = 0;
		if (boundary.size() && buffer.find(boundary) != -1)
		{
			std::cout << "Boundary found : one  " << boundary << std::endl;
			return (0);
		}
	}
	else if (boundary.size() && buffer.find(boundary) != -1)
	{
		std::cout << "Boundary found : two " << boundary << std::endl;
		return (0);
	}
	if (content_len >= 0 && contentLength(buffer, content_len))
	{
		std::cout << "content Length reached" << std::endl;
		return (0);
	}
	if (flag && content_len == -1 && buffer.size() >= 4)
	{
		if (buffer.compare(buffer.size() - 4, 4, "\r\n\r\n") == 0)
		{
			std::cout << "rrrsss\n" ;
			return (0);
		}
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
	int								flag;
	std::string						boundary;
	int								fd;
	int								rd;
	char							line[100000];
	std::stringstream				buffer;
	std::vector<client>::iterator	it;
	unsigned long					content_len;
	struct body						bodys;
	
	flag = 1;
	content_len = 0;
	bodys.chunks_flag = 0;
	bodys.boundary_flag = 0;
	bodys.content_length_flag = 0;
	bodys.cr_nl_flag = 0;
	bodys.chunks_len = 0;
	bodys.content_len = 0;
	i = 0;
	while (i < web.clients.size())
	{
		it = web.clients.begin();
		fd = web.clients[i].fd;
		std::cout << "FD :" << fd << std::endl;
		if (FD_ISSET(fd, &web.cReads))
		{
			while (1)
			{
				rd = 0;
				memset(line, 0, 100000);
				rd = recv(web.clients[i].fd, line, 99999, 0);
				line[rd] = 0;
				std::cout << "*****************************" << std::endl;
				std::cout << "rd: {" << rd << "} Line [" << line << "]" << std::endl;
				std::cout << "*****************************" << std::endl;
				if (rd > 0)
					buffer << line;
				if (rd == -1)
				{
					std::cerr << "Error in recv" << std::endl;;
					return;
				}
				if (handleContinue(line) == 0)
				{
					std::cout << CONTINUE << std::endl;
					send(web.clients[i].fd, CONTINUE, strlen(CONTINUE), 0);
				}
				else if (endOfTheRequest(buffer.str(), bodys, flag, boundary, content_len) == 0)
				{
					std::cout << "go Get out" << std::endl;
					break;
				}
				if (rd == 0)
				{
					std::cout << "*--------------------------*" << std::endl;
					std::cout << buffer << std::endl;
					std::cout << "*--------------------------*" << std::endl;
					break;
				}
			}
			if (rd == 0)
			{
				std::cout << "connection is end" << std::endl;
				closeConnection(web, it, i);
			}
			else
			{
				std::cout << "Clients request : " << std::endl;
				std::cout << " ----------------------------" << std::endl;
				std::cout << buffer.str() << std::endl;;
				parseRequests(web, buffer);
				std::cout << " ----------------------------" << std::endl;
			//	parseRequest(web, web.clients[i], buffer);
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
