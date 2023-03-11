/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 17:35:47 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/11 16:25:53 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


void	getMethod(std::vector<std::pair<std::string, std::string> >& req, char *line)
{
	char	**keys;
	int		index;
	std::pair<std::string, std::string> pr;
	std::vector<std::pair<std::string, std::string> >::iterator it;

	index = 0;
	line[strlen(line) - 1] = 0;
	keys = ft_split(line, ' ');
	if (keys == NULL)
		return;
	while (keys[index])
	{
		index++;
	}
	if (index > 0)
	{
		pr.first = "Method";
		pr.second = keys[0];
		req.push_back(pr);
	}
	if (index > 1)
	{
		pr.first = "Uri";
		pr.second = keys[1];
		req.push_back(pr);
	}
	if (index > 2)
	{
		pr.first = "Version";
		pr.second = keys[2];
		req.push_back(pr);
	}
}

void	getHeaders(std::vector<std::pair<std::string, std::string> >& req, char *line)
{
	char	**keys;
	int		index;
	std::pair<std::string, std::string> pr;
	std::vector<std::pair<std::string, std::string> >::iterator it;

	index = 0;
	line[strlen(line) - 1] = 0;
	if (strncmp(line, "Referer:", 8) == 0)
	{
		line[7] = ' ';
		keys = ft_split(line, ' ');
	}
	else
		keys = ft_split(line, ':');
	if (keys != NULL)
	{
		if (keys[index])
		{
			pr.first = keys[index];
			if (keys[index + 1] != NULL)
				pr.second = keys[index + 1];
			else
				pr.second = "";
			req.push_back(pr);
			if (pr.first == "Host")
			{
				pr.first = "Port";
				if (keys[0] != NULL && keys[1] != NULL)
					pr.second = keys[index + 2];
				req.push_back(pr);
			}
		}
		free(keys);
	}
}

void	displyReqeust(std::vector<std::pair<std::string, std::string> > req)
{
	std::vector<std::pair<std::string, std::string> >::iterator it;

	it = req.begin();
	while (it != req.end())
	{
		std::cout << "[ " << it->first << " ] : [ " << it->second << " ]"<< std::endl;
		it++;
	}
}

void	getRequestInfo(struct webserv& web, char* buffer)
{
	char	**lines;
	int		index;
	
	if (web.reqIndex == 10)
		web.reqIndex = 0;
	index = 0;
	if (buffer != NULL)
	{
		lines = ft_split(buffer, '\n');
		if (lines != NULL)
		{
			while (lines[index])
			{
				if (index == 0)
				{
					getMethod(web.request[web.reqIndex], lines[index]);
				}
				else
				{
					getHeaders(web.request[web.reqIndex], lines[index]);
				}
				index++;
			}
			free(lines);
		}
		web.reqIndex++;
	}
	else
		printf("buffer is empty\n");
	index = 0;
	while (index < web.reqIndex)
	{
		std::cout << "----------------------------------" << std::endl;
		displyReqeust(web.request[index]);
		std::cout << "----------------------------------" << std::endl;
		index++;
	}
}
