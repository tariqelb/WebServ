/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:43:10 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/21 19:17:42 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

void	removeEmptyLineAndComments(std::vector<std::string>& file)
{
	std::vector<std::string>::iterator 	it;
	int									index;

	index = 0;
	it = file.begin();
	while (index < file.size())
	{
		if (isEmptyLines(file[index]) == 0)
		{
			file.erase(it + index);
			it = file.begin();
			index--;
		}
		else if (isComment(file[index]))
		{
			file.erase(it + index);
			it = file.begin();
			index--;
		}
		index++;
	}
}

void	countServerBlock(std::vector<std::string> file, int& nbr_of_server)
{
	int 		i;
	int			size;

	size = file.size();
	i = 0;
	while (i < size)
	{
		if (isServerBlock(file[i]))
			nbr_of_server++;
		i++;
	}
}

int	nbrOfLocBlock(std::vector<std::string> file)
{
	int i;
	int	size;
	int count;

	size = file.size();
	i = 0;
	count = 0;
	while (i < size)
	{
		if (isLocationBlock(file[i]))
			count++;
		i++;
	}
	return (count);
}


int	closedBlock(std::vector<std::string> file)
{
	int i;
	int size;
	int	flag;
	int	nbr_of_loc_block;

	nbr_of_loc_block = 0;
	size = file.size();
	flag = 0;
	if (size > 3 && isServerBlock(file[0]) && isBracket(file[1]) == 1 && isBracket(file[size - 1]) == 2)
	{
		nbr_of_loc_block = nbrOfLocBlock(file);
		i = 0;
		while (i < size)
		{
			if (isLocationBlock(file[i]))
			{
				std::vector<std::string> loc;
				int k = i;
				while (k < size && isBracket(file[k]) != 2)
				{
					loc.push_back(file[k]);
					k++;
				}
				loc.push_back(file[k]);
				if (loc.size() > 3 && isBracket(loc[1]) == 1 && isBracket(loc[loc.size() - 1]) == 2)
					flag++;
			}
			i++;
		}
		if (nbr_of_loc_block == flag)
			return (1);
		return (0);
	}
	else
		return (0);
}

int	parseConfigData(std::vector<std::string>& file)
{
	removeEmptyLineAndComments(file);
	moveBracketsToNextLine(file);//only brackets thats come after server or location block
	splitSemiColons(file);
	removeEmptySemiColons(file);
	//if (checkForPairBrackets(file)) // check for closed block
	//	if (checkForSemiColon(file)) // ckeck for ends with semi-colon
	//		return (0);
	//		check for valide directive
	//		check if at least there a valide server neccesary directive and valide value
	//
	return (0);
}

void	parseConfigFile(struct webserv& web, int ac, char **av)
{
	std::ifstream				file;
	const char 					*filename;
	std::string					line;
	std::vector<std::string>	conf_file;//fill in with line from server.conf

	if (ac == 2)
		filename = av[1];
	else
		filename = CONFIGFILE;
	file.open(filename);
	if (file.is_open() == false)
	{
		write(2, "Error opening config file\n", 26);
		web.status = 1;
		return ;
	}
	while (getline(file, line))
	{
		conf_file.push_back(line);
	}
	file.close();
	if (conf_file.size() == 0)
	{
		write(2, "Error Empty file\n", 17);
		web.status = 1;
		return ;
	}
	if (parseConfigData(conf_file))
	{
		write(2, "Error in config file\n", 21);
		web.status = 1;
		return;
	}
	//test
	int i = 0;
	while (i < conf_file.size())
	{
		std::cout << "Line : " << conf_file[i] << std::endl; 
		i++;
	}
	//getConfigData(web, conf_file);
	//checkConfigData(web);
}
