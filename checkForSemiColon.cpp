/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkForSemiColon.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 17:06:21 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/19 17:29:22 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	checkForSemiColon(std::vector<std::string> file)
{
	int i;
	int	size;

	i = 0;
	size = file.size();
	while (i < size)
	{
		if (!isServerBlock(file[i]) && !isLocationBlock(file[i]) && isBracket(file[i]) == 0)
		{
			int len = file[i].size() - 1;
			std::string line = file[i];
			while (len >= 0 && (line[len] == ' ' || line[len] == '\t'))
				len--;
			if (line[len] != ';')
				return (0);
		}	
		i++;
	}
	return (1);
}
