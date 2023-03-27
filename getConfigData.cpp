/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getConfigData.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 17:44:50 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/27 17:55:34 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"



void	displayConfigFile(std::vector<struct serverfile> conf)
{
	int i;

	i = 0;
	while (i < conf.size())
	{
		std::cout << "***********************************" << std::endl;
		{
			int j = 0;
			while (j < conf[i].listen.size())
			{
				std::cout << "listen : " << conf[i].listen[j] << std::endl;
				j++;
			}
		}
		std::cout << "___________________" << std::endl;
		{
			int j = 0;
			while (j < conf[i].server_name.size())
			{
				std::cout << "servername : " << conf[i].server_name[j] << std::endl;
				j++;
			}
		}
		std::cout << "___________________" << std::endl;
		{
			int j = 0;
			while (j < conf[i].error_page.size())
			{
			std::cout << "error_code: " << conf[i].error_page[j].first;
			std::cout << " error_page: " << conf[i].error_page[j].second;
			std::cout << std::endl;
				j++;
			}
		}
		std::cout << "___________________" << std::endl;
		std::cout << "Max Body Size : " << conf[i].max_body_size << "\n";
		std::cout << "___________________" << std::endl;
		std::cout << "Root : " << conf[i].root << std::endl;
		std::cout << "___________________" << std::endl;
		std::cout << "Index : " << conf[i].index << std::endl;
		std::cout << "-------------------" << std::endl;	
		
		i++;
	}
}



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

void	getKeyandValue(std::string& key, std::string& value, std::string line)
{
	int			size;
	int			i;
	int			j;
	int			k;

	i = 0;
	size = line.size();
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	j = 0;
	while (i + j < size  && line[i + j] != ' ' && line[i + j] != '\t')
		j++;
	if (j)
		key = line.substr(i, j);
	i = i + j;
	while (i < size && (line[i] == ' ' || line[i] == '\t'))
		i++;
	j = size - 1;
	while (j > 0 && (line[j] == ' ' || line[j] == '\t' || line[j] == ';'))
		--j;
	if (j > i)
		value = line.substr(i, j - i + 1);
}


void	getMultivalue(std::vector<std::string>& holder, std::string values)
{
	int i;
	int j;
	int size;
	int index;

	index = 0;
	size = values.size();
	i = 0;
	while (i < size)
	{
		while (i < size && (values[i] == ' ' || values[i] == '\t'))
			i++;
		j = 0;
		while (i + j < size  && values[i + j] != ' ' && values[i + j] != '\t')
			j++;
		if (j)
		{
			holder.push_back(values.substr(i , j));
			values.assign(values.substr(i + j, size - (i + j)));
			size = values.size();
			i = 0;
		}
	}
}


void	fillServerBlock(struct webserv& web, std::vector<std::string> serv)
{
	int 					i;
	int 					size;
	struct serverfile		conf;
	size = serv.size();
	i = 0;	
	while (i < size)
	{
		std::string		key;
		std::string		value;

		getKeyandValue(key, value, serv[i]);
		if (key == "listen")
		{
			//std::cout << "Line : " << serv[i] << std::endl;
			conf.listen.push_back(value);
		}
		if (key == "servername")
		{
			getMultivalue(conf.server_name, value);
		}
		if (key == "client_max_body_size")
		{
			conf.max_body_size.assign(value);
		}
		if (key == "root")
		{
			conf.root.assign(value);
		}
		if (key == "index")
		{
			conf.index.assign(value);
		}
		i++;
	}
	web.config.push_back(conf);
	displayConfigFile(web.config);
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
		else
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
