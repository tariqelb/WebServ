/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handleConnection.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:04:07 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/08 19:49:40 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

char    temp[2048] = "HTTP/1.0 200 OK\r\n Server: webserver-c\r\n Content-type: text/    html\r\n\r\n <html>hello, world    </html>\r\n";


void	closeConnection(struct webserv& web, int client_i)
{
   	std::vector<client>::iterator it;

	it = web.clients.begin();
	FD_CLR(web.clients[client_i].fd , &web.writes);
	close(web.clients[client_i].fd);
	while (client_i < web.clients.size() && (*it).fd != web.clients[client_i].fd && it != web.clients.end())
		it++;
	if (it != web.clients.end())
		web.clients.erase(it);
	maxFd(web);
}
/*
void	handleConnection(struct webserv& web)
{
	struct client 	newClient;
	int				i;
	int				j;
	int				size;
	int				k;
	
	size = web.servers.size();	
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			if (FD_ISSET(web.servers[i].socketFd[j], &web.tmp_read))
			{
					newClient.len = sizeof(newClient.addr);
					newClient.fd = accept(web.servers[i].socketFd[j], (struct sockaddr *)&newClient.addr, &newClient.len);
					if (newClient.fd < 0)
					{
						std::cerr << "Error : Fail connecting to client" << std::endl;
						return ;
					}
					else
					{
						FD_SET(newClient.fd, &web.reads);
						web.clients.push_back(newClient);
						k = web.clients.size() - 1;
						web.clients[k].bodys.chunks_flag = 0;
						web.clients[k].bodys.boundary_flag = 0;
						web.clients[k].bodys.content_length_flag = 0;
						web.clients[k].bodys.cr_nl_flag = 0;
						web.clients[k].bodys.chunks_len = 0;
						web.clients[k].bodys.chunks_con_len = 0;
						web.clients[k].bodys.content_len = 0;
						web.clients[k].bodys.n_chunks = 0;
						web.clients[k].bodys.cr_index = -1;
						web.clients[k].bodys.get_body_type = 0;
						web.clients[k].file_name = "req" + std::to_string(web.req_nbr) + ".txt";
						std::cout << "filename : " << web.clients[k].file_name << std::endl;
						web.req_nbr++;
						maxFd(web);
						return ;
					}
			}
			j++;
		}
		i++;
	}
	size = web.clients.size();	
	std::cout << "data 0 :  " <<  i << " " << web.next_read <<  " " << size << std::endl; 
	i = web.next_read;
	std::cout << "data 1 :  " <<  i << " " << web.next_read <<  " " << size << std::endl; 
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_read))
		{
			std::cout << "Index 1 : " << i << std::endl;
			receiveRequest(web, web.clients[i], i);	
			if (web.clients[i].request_is_ready == true)
			{
				FD_CLR(web.clients[i].fd , &web.reads);

				//parseRequest(web, web.clients[i]);
			}
		}
		i++;
	}
	std::cout << "data 2 :  " <<  i << " " << web.next_read <<  " " << size << std::endl; 
	i = 0;
	while (i < size && i < web.next_read)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_read))
		{
			std::cout << "Index 2 : " << i << std::endl;
			receiveRequest(web, web.clients[i], i);
			
			if (web.clients[i].request_is_ready == true)
			{
				FD_CLR(web.clients[i].fd , &web.reads);

				//parseRequest(web, web.clients[i]);
			}
		}
		i++;
	}
	std::cout << "data 3 :  " <<  i << " " << web.next_read <<  " " << size << std::endl; 
	web.next_read++;
	std::cout << "data 4 :  " <<  i << " " << web.next_read <<  " " << size << std::endl; 
	if (web.next_read == size)
		web.next_read = 0;
	std::cout << "data 5 :  " <<  i << " " << web.next_read <<  " " << size << std::endl;
	i = 0;
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_write) )
		{
			if (web.clients[i].request_is_ready == true)// * && web.clients[i].response_is_ready == true * /)
			{
				send(web.clients[i].fd, temp, strlen(temp), 0);
				closeConnection(web, i);
			}
		}
		i++;
	}
}
*/

void	handleConnection(struct webserv& web)
{
	struct client 	newClient;
	int				i;
	int				j;
	int				size;
	int				k;
	
	size = web.servers.size();	
	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < web.servers[i].socketFd.size())
		{
			if (FD_ISSET(web.servers[i].socketFd[j], &web.tmp_read))
			{
					newClient.len = sizeof(newClient.addr);
					newClient.fd = accept(web.servers[i].socketFd[j], (struct sockaddr *)&newClient.addr, &newClient.len);
					if (newClient.fd < 0)
					{
						std::cerr << "Error : Fail connecting to client" << std::endl;
						return ;
					}
					else
					{
						FD_SET(newClient.fd, &web.reads);
						web.clients.push_back(newClient);
						k = web.clients.size() - 1;
						web.clients[k].bodys.chunks_flag = 0;
						web.clients[k].bodys.boundary_flag = 0;
						web.clients[k].bodys.content_length_flag = 0;
						web.clients[k].bodys.cr_nl_flag = 0;
						web.clients[k].bodys.chunks_len = 0;
						web.clients[k].bodys.chunks_con_len = 0;
						web.clients[k].bodys.content_len = 0;
						web.clients[k].bodys.n_chunks = 0;
						web.clients[k].bodys.cr_index = -1;
						web.clients[k].bodys.get_body_type = 0;
						web.clients[k].file_name = "req" + std::to_string(web.req_nbr) + ".txt";
						std::cout << "filename : " << web.clients[k].file_name << std::endl;
						web.req_nbr++;
						maxFd(web);
						return ;
					}
			}
			j++;
		}
		i++;
	}
	size = web.clients.size();	
	i = 0;
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_read))
		{
			std::cout << "Index: " << i << " "<< web.clients[i].fd << " " << web.clients[i].file_name  << std::endl;
			receiveRequest(web, web.clients[i], i);
			
			if (web.clients[i].request_is_ready == true)
			{
				FD_CLR(web.clients[i].fd , &web.reads);
				int returnValue = parseRequest(web, web.clients[i]);
				if (returnValue == 200 && web.clients[i].map_request["Method"] == "GET")
					get(web, web.clients[i]);
				else if (returnValue == 200 && web.clients[i].map_request["Method"] == "POST")
					post(web, web.clients[i]);
				else if (returnValue == 200 && web.clients[i].map_request["Method"] == "DELETE")
					deleteResponse(web, web.clients[i]);
				std::cout << "\033[00m";
				//parseRequest(web, web.clients[i]);
			}
		}
		i++;
	}
	i = 0;
	while (i < size)
	{
		if (FD_ISSET(web.clients[i].fd, &web.tmp_write) )
		{
			if (web.clients[i].request_is_ready == true)// * && web.clients[i].response_is_ready == true *//*)
			{
				// send(web.clients[i].fd, temp, strlen(temp), 0);
				closeConnection(web, i);
			}
		}
		i++;
	}
}
