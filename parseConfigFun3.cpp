/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfigFun3.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 21:10:34 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/29 21:10:56 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

//split file content to several server block then check for each has pair number of brackets
int	checkForPairBrackets(std::vector<std::string>& file)
{
	int	i;
	int	j;
	int	size;
	int	nbr_of_server;
	int	flag;

	nbr_of_server = 0;
	countServerBlock(file, nbr_of_server);
	std::vector<std::string> servers[nbr_of_server];
	size = file.size();
	j = 0;
	i = 0;
	while (i < size)
	{
		if (isServerBlock(file[i]))
		{
			servers[j].push_back(file[i]);
			i++;
			while (i < size &&  !isServerBlock(file[i]))
				servers[j].push_back(file[i++]);
			j++;
		}
		else
			i++;
	}
	i = 0;
	flag = 0;
	while (i < nbr_of_server)
	{
		flag += pairBrackets(servers[i]);//check if each server have pair number of curly brackets
		i++;
	}
	if (flag == nbr_of_server)
	{
		i = 0;
		flag = 0;
		while (i < nbr_of_server)
		{
			flag += closedBlock(servers[i]);
			i++;
		}
		if (flag == nbr_of_server)
			return (1);
		return (0);
	}
	return (0);
}

//check if the number of curly brackets is pair
int	pairBrackets(std::vector<std::string> serv)
{
	int i;
	int	size;
	int nbr_bracket;

	i = 0;
	size = serv.size();
	while (i < size)
	{
		if (isBracket(serv[i]))
			nbr_bracket++;
		i++;
	}
	if (nbr_bracket % 2 == 0)
		return (1);
	return (0);
}
