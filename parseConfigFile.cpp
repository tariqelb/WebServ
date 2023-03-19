/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfigFile.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 16:43:10 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/19 17:29:26 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>

/*
int	main(int ac, char **av)
{
	std::string line;

	if (ac != 2)
		return (1);
	std::ifstream	config;
	config.open(av[1], std::ios::in);

	while (getline(config, line))
	{
		std::cout << "The line is : " << line << std::endl;

	}
	std::cout << "The End of file " << std::endl;
	config.close();
	return (0);
}
*/

int	isEmptyLines(std::string line)
{
	int	i;

	i = 0;
	if (line.size() == 0)
		return (0);
	while (i < line.size() && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (i == line.size())
		return (0);
	return (1);
}


void	checkTitle(std::string line, int& title, int& brackets)
{
	int i;

	i = 0;
	while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) 
			i++;
	if (line.compare(i, 6, "server") == 0)
		title = 1;
	if (title)
	{
		i = i + 6;
		while (i < line.size() && (line[i] == ' ' || line[i] == '{'))
		{
			if (line[i] == '{')
				brackets++;
			i++;
		}
		if (line[i] != 0)
			brackets = -1;
	}
}

int	parseSyntax(std::ifstream& file)
{
	std::string		line;
	int				brackets;
	int				title;
	int				semi_colon;

	title = 0;
	brackets = 0;
	getline(file, line);
	if (line.size() == 0)
	{
		write(2 , "Error config file is empty\n", 27);
		return (1);
	}
	while (isEmptyLines(line) == 0)
	{
		if (getline(file, line) == 0)
			return (1);
	}
	checkTitle(line, title, brackets);
	if (!title)
	{
		write(2 , "Error config file is empty\n", 27);
		return (1);
	}
	std::cout << "T : " << title << "  B : " << brackets << std::endl;
	return (0);	
}


int	isComment(std::string line)
{
	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
	i++;
	if (line[i] == '#')
		return (1);
	return (0);
}


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

int	isaBlockWithBrackets(std::string& line)
{
	int	i;
	int	size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line.compare(i, 6, "server") == 0)
	{
		i = i + 6;
		while (i < size && (line[i] == ' ' || line[i] == '\t'))
			i++;
		if (line[i] == '{')
		{
			line[i] = ' ';
			return (1);
		}
		return  (0);
	}
	if (line.compare(i, 8, "location") == 0)
	{
		i = size - 1;
		while (i > 0 && (line[i] == ' ' || line[i] == '\t'))
			i--;
		if (line[i] == '{')
		{
			line[i] = ' ';
			return (2);
		}
		return  (0);
	}
	return  (0);
}


int	isDirectiveWithbrackets(std::string& line)
{
	int	i;
	int	size;

	size = line.size() - 1;
	i = 0;
	while (size >= 0 && (line[size] == ' ' || line[size] == '\t'))
		size--;
	if (line[size] == '}')
	{
		if (size == 0)
			return (0);
		i = size;
		--size;
		while (size >= 0 && (line[size] == ' ' || line[size] == '\t'))
			size--;
		if (size == -1)
			return (0);
		else
		{
			line[i] = ' ';
			return (1);
		}
	}
	return (0);
}

void	moveBracketsToNextLine(std::vector<std::string>& file)
{
	int	i;
	int	size;
	int	tab;
	std::vector<std::string>::iterator it;

	//first part is to move open curly bracket in
	//server and location blocks to a new line
	it = file.begin();
	size = file.size();
	tab = 0;
	i = 0;
	while (i < size)
	{
		if ((tab = isaBlockWithBrackets(file[i])))
		{
			//move it to the next line;
			if (tab == 1)
				file.insert(it + i + 1, "{");
			else
				file.insert(it + i + 1, "\t{");
			size = file.size();
			it = file.begin();
		}
		i++;
	}
	//secound part is to move close curly brackets in
	//the ends of directive value to a new line
	it = file.begin();
	size = file.size();
	i = 0;
	tab = 0;
	while (i < size)
	{
		if ((tab = isDirectiveWithbrackets(file[i])))
		{
			file.insert(it + i + 1, "\t}");
			size = file.size();
			it = file.begin();
		}
		i++;
	}
}

int	isServerBlock(std::string line)
{
	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line.compare(i, 6, "server") == 0)
	{
		if (line[i + 6] == ' ' || line[i + 6] == '\t' || line[i + 6] == 0)
			return (1);
	}
	return (0);
}
int	isLocationBlock(std::string line)
{
	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line.compare(i, 8, "location") == 0)
	{
		if (line[i + 8] == ' ' || line[i + 6] == '\t')
			return (1);
	}
	return (0);

}
int	isBracket(std::string line)
{
	int i;
	int size;

	size = line.size();
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '{')
		return (1);
	if (line[i] == '}')
		return (2);
	return (0);

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

void	parseConfigData(std::vector<std::string>& file)
{
	removeEmptyLineAndComments(file);
	moveBracketsToNextLine(file);//only brackets thats come after server or location block
	if (checkForPairBrackets(file))
	{
		int flag = checkForSemiColon(file);
		std::cout << "from here : " << flag << std::endl;
		/*{
			std::vector<std::string>::iterator it = file.begin();
			while (it != file.end())
			{
				std::cout << *it << std::endl;
				it++;
			}

		}*/
	}
}

/*void	getConfigData(struct webserv& web, char *filename)
{
	std::ifstream	file;

	file.open(filename);
	if (parseSyntax(file))
	{
		write(2, "Error in file syntax\n", 21);
		web.status = 1;
		return ;
	}
}*/

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
	parseConfigData(conf_file);
	//getConfigData(web, conf_file);
	//checkConfigData(web);
}
