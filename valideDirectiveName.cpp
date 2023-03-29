/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valideDirectiveName.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 10:58:07 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/27 22:37:16 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"


char dirctives[14][21] = {"listen", "servername", "error_page", "client_max_body_size", "location", "allow", "root", "autoindex", "index", "upload", "upload_store", "cgi", "script_filename"};


std::string	getDirectiveKey(std::string line)
{
	int i;
	int size;
	int pos;
	int len;

	size = line.size();
	pos = 0;
	len = 0;
	i = 0;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	pos = i;
	while (i < size && line[i] != ' ' && line[i] != '\t')
	{
		i++;
		len++;
	}
	return (line.substr(pos, len));
}

std::string getDirectiveValue(std::string line, std::string key)
{
	int	i;
	int size;
	int index;
	int	end;
	int len;

	size = line.size();
	end = size;
	while (end > 0 && (line[end - 1] == ' ' || line[end - 1] == '\t' || line[end - 1] == ';'))
			end--;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	i = i + key.size();
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	return (line.substr(i, end - i));
}

int	valideDirectiveValue(std::string line, std::string key)
{
	int 		i;
	int 		size;
	std::string value;

	value = getDirectiveValue(line, key);
	std::cout << "The value is : " << value << std::endl;
	if (key == "listen" && validePort(value) == 0)
		return (0);
	if (key == "servername" && valideDomainName(value) == 0)
		return (0);
	if (key == "error_page" && valideFile(value) == 0)
		return (0);
	if (key == "client_max_body_size" && valideUnit(value) == 0)
		return (0);
	if (key == "autoindex" && valideOnOff(value) == 0)
		return (0);
	if (key == "upload" && valideOnOff(value) == 0)
		return (0);
	if (key == "cgi" && valideExtension(value) == 0)
		return (0);
	if (key == "script_filename" && valideScript(value) == 0)
		return (0);
	if (key == "upload_store" && validePath(value) == 0)
		return (0);
	if (key == "root" && validePath(value) == 0)
		return (0);
	if (key == "location" && validePath(value) == 0)
		return (0);
	return (2);
}

int valideDirectiveName(std::vector<std::string> file)
{
	int i;
	int size;
	int	flag;
	int	open_bracket;

	size = file.size();
	i = 0;
	while (i < size)
	{
		open_bracket = 0;	
		if (!isServerBlock(file[i]) && isBracket(file[i]) == 0)
		{
			int j;
			std::string key;
			flag = 0;
			key = getDirectiveKey(file[i]);
			j = 0;
			while (j < 13 && flag == 0)
			{
				if (key.compare(0, key.size() , dirctives[j]) == 0)
					flag = 1;
				j++;	
			}
			if (flag == 0)
			{
				std::cout << "unvalide Directive key " << key << std::endl;
				return (1);
				
			}
			/*if (valideDirectiveValue(file[i], key) == 0)
			{
			std::cout << "unvalide Directive value " << file[i] << std::endl;
				return (1);	
			}*/
		}
		i++;
	}
	return (0);
}
