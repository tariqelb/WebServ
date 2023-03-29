/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfigFun2.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 12:46:10 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/29 00:32:38 by tel-bouh         ###   ########.fr       */
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
		if ((tab = isaBlockWithOpenBrackets(file[i])))
		{
			//move it to the next line;
			if (tab > 0)
			{
				file.insert(it + i + 1, "{");
				if ((file[i].substr(tab + 1, file[i].size() - tab)).size())
					file.insert(it + i + 2, file[i].substr(tab + 1, file[i].size() - tab));
				file[i].assign(file[i].substr(0, tab));
			}
			else
			{
				tab = tab * - 1;
				file.insert(it + i + 1, "\t{");
				if ((file[i].substr(tab + 1, file[i].size() - tab)).size())
					file.insert(it + i + 2, file[i].substr(tab + 1, file[i].size() - tab));
				file[i].assign(file[i].substr(0, tab));
			}
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
	while (i < size)
	{
		tab = 0;
		int find = file[i].find("}");
		if (find  >= 0)
		{
			tab = isDirectiveWithCloseBrackets(file[i]);
			if (tab == 1)
			{
				file[i].assign(file[i].substr(0, find));
				file.insert(it + i + 1, "\t}");
			}
			else if (tab == 2)
			{
				file.insert(it + i + 1, file[i].substr(find + 1, file[i].size() - find));
				file[i].assign("}");
			}	
			else if (tab == 3)
			{
				file.insert(it + i + 1, "\t}");
				size = file.size();
				it = file.begin();
				file.insert(it + i + 2, file[i].substr(find + 1, file[i].size() - find));
				file[i].assign(file[i].substr(0, find));
				
			}
			size = file.size();
			it = file.begin();
		}
		i++;
	}
}

//check if server or location blocks are in the same line with curly brackets
int	isaBlockWithOpenBrackets(std::string& line)
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
		if (line[i] == 0)
			return (0);
		while (i < size && line[i] != '{')
				i++;
		if (line[i] == '{')
				return (i);
		return  (0);
	}
	if (line.compare(i, 8, "location") == 0)
	{
		i = size - 1;
		while (i > 0 && line[i] != '{')
			i--;
		if (line[i] == '{')
			return (i * -1);
		return  (0);
	}
	return  (0);
}

//ckeck if close curly bracket is alone in a line 
//return 0 alone 
//return 1 data before curly bracket
//return 2 data after curly bracket
//return 3 data before and after curly bracket
int	isDirectiveWithCloseBrackets(std::string& line)
{
	int	i;
	int	size;
	int find;
	int	flag;

	size = line.size();
	flag = 0;
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] != '}')
		flag = 1;
	while (i < size && line[i] != '}')
		i++;
	if (line[i] == '}')
		i++;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (i == size)
		return (flag);
	if (i != size)
	{
		if (flag)
			flag = 3;
		else
			flag = 2;
	}	
	return (flag);
}

