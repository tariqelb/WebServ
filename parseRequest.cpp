/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseRequest.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 21:03:09 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/04/13 00:41:58 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


int	isWhileSpace(int c)
{
	if (c == ' ' || c == '\n' || c == '\r' || c == ':')
		return (1);
	return (0);
}

int	isRequestWellFormed(struct webserv web, std::string buff)
{
	return (1);
}

void	getMethodUriVersion(struct client& clt, std::string buff)
{
	int i;
	int size;
	std::pair<std::string, std::string> pr;

	//Get Method -----------------------------------------
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = "Method";
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);
	pr.first.assign("");
	pr.second.assign("");
	while (i < size && isWhileSpace(buff[i]))
		i++;
	buff = buff.substr(i, buff.size());
	//Get Uri  -----------------------------------------
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = "Uri";
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);
	pr.first.assign("");
	pr.second.assign("");
	while (i < size && isWhileSpace(buff[i]))
		i++;
	buff = buff.substr(i, buff.size());
	//Get Http -----------------------------------------
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = "Http";
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);

}
void	getHostPort(struct client& clt, std::string buff)
{
	int i;
	int size;
	std::pair<std::string, std::string> pr;

	//Get Host and Port -------------------------------------
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = buff.substr(0, i);
	while (i < size && isWhileSpace(buff[i]))
		i++;
	buff = buff.substr(i, buff.size());
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);
	pr.first.assign("");
	pr.second.assign("");
	while (i < size && isWhileSpace(buff[i]))
		i++;
	buff = buff.substr(i, buff.size());
	i = 0;
	size = buff.size();
	while (i < size && !isWhileSpace(buff[i]))
		i++;
	pr.first = "Port";
	pr.second = buff.substr(0, i);
	clt.request.push_back(pr);
}

void	getKeyValue(struct client& clt, std::string buff)
{
	int find;
	int size;
	int i;
	int j;
	std::pair<std::string, std::string> pr;
		
	size = buff.size();
	find = buff.find(":");
	if (find > 0)
	{
		pr.first = buff.substr(0, find);
		i = find + 1;
		while (i < size && (buff[i] == ' ' || buff[i] == '\t'))
			i++;
		if (size > i)
			pr.second = buff.substr(i, find);
		clt.request.push_back(pr); 
	}
}

void	fillRequestData(struct client& clt, std::stringstream& buffer)
{
	int i;
	int size;
	std::pair<std::string, std::string> pr;
	std::string buff;

	i = 0;
	getline(buffer, buff);
	while (buff.size())
	{
		if (i == 0)
			getMethodUriVersion(clt, buff);
		else if (buff.compare(0, 6, "Host: ") == 0)
			getHostPort(clt, buff);
		else if (buff == "\r")
		{
			//get the body
			//Create another body key val if you reach max std::string size;
			getline(buffer, buff);
			pr.first = "Body";
			while (buff.size())
			{
				buff += "\n";
				pr.second += buff;
				buff.assign("");
				getline(buffer, buff);
			}
			clt.request.push_back(pr);
		}
		else if (buff.size())
			getKeyValue(clt, buff);
		buff.assign("");
		getline(buffer, buff);
		i++;
	}
	i = 0;
	size = clt.request.size();
	while (i < size)
	{
		std::cout << "Key : " << clt.request[i].first   << std::endl;
		std::cout << "val : " << clt.request[i].second  << std::endl;
		i++;
	}
}

void	parseRequest(struct webserv web, struct client& clt, std::stringstream& buff)
{
	fillRequestData(clt, buff);
	/*if (isRequestWellFormed(web, buff) == 0)
	{
		std::cerr << "Request not well formed" << std::endl;
	}*/
}
