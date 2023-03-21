/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getConfigData.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 17:44:50 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/20 11:38:35 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	nbrOfServerBlock(std::vector<std::string> file)
{
	int i;
	int size;
	int count;

	count = 0;
	size = file.size();
	while (i < size)
	{
		if (isServerBlock(file[i]))
			count++;
		i++;
	}
	return (count);
}

void	getKey(std::string& key, std::string line)
{
	int	i;
	int	j;
	char tmp[30];
	int size;

	i = 0;
	j = 0;
	size = line.size();
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	while (i < size && j < 30 && line[i] != ' ' && line[i] != '\t')
	{
		tmp[j] = line[i];
		i++;
		j++;
	}
	tmp[j] = 0;
	key.assign(tmp);
}

void	fillServerBlock(struct webserv& web, std::vector<std::string> serv)
{
	int i;
	int size;

	size = serv.size();
	i = 0;	
	while (i < size)
	{
		std::string key;
		getKey(key, serv[i]);
		std::cout << "Key : " << key << std::endl;
		i++;
	}
}

void	getConfigData(struct webserv& web, std::vector<std::string> file)
{
	int i;
	int size;
	
	size = file.size();
	i = 0;
	while (i < size)
	{
		if (isServerBlock(file[i]))
		{
			std::vector<std::string> serv;
			serv.push_back(file[i]);
			i++;
			while (i < size && !isServerBlock(file[i]))
				serv.push_back(file[i++]);
			fillServerBlock(web, serv);
		}
		i++;
	}
	/*size = file.size();
	i = 0;
	while (i < size)
	{
		std::cout << file[i] << std::endl;
		i++;
	}*/
}
