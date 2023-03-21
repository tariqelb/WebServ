/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parseConfigFun1.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 11:40:24 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/21 18:47:29 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


//check if a line have only whitespace characters return (0) if yes otherwise return (1)
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



//check if the line start with # so its a comment that will be removed
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

//check if a line starts with "server" so it will consider as start of server block
int	isServerBlock(std::string line)
{
	int find;
	int	i;

	find = line.find("server");
	if (find >= 0)
	{
		i = find + 6;//Index of the character after server word in the string
		if (find == 0 && (line[i] == ' ' || line[i] == '\t' || line[i] == 0 ))
			return (1);
		if (find > 0)
		{
			if ((line[find - 1] == ' ' || line[find - 1] == '\t') 
					&& (line[i] == ' ' || line[i] == '\t' || line[i] == 0))
				return (1);
		}
	}
	return (0);

/*	int i;
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
	return (0);*/
}
// check if a line starts with loaction block so its the start of locaion block
int	isLocationBlock(std::string line)
{
	int find;
	int	i;

	find = line.find("server");
	if (find >= 0)
	{
		i = find + 6;//Index of the character after server word in the string
		if (find == 0 && (line[i] == ' ' || line[i] == '\t' || line[i] == 0 ))
			return (1);
		if (find > 0)
		{
			if ((line[find - 1] == ' ' || line[find - 1] == '\t') 
					&& (line[i] == ' ' || line[i] == '\t' || line[i] == 0))
				return (1);
		}
	}
	return (0);

/*	int i;
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
	*/

}
//check if a line start with open or close curly brackets return (1) open return (2) close
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
