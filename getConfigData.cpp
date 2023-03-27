/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getConfigData.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 17:44:50 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/27 22:35:03 by tel-bouh         ###   ########.fr       */
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
		{
			int j = 0;
			while (j < conf[i].server_name.size())
			{
				std::cout << "servername : " << conf[i].server_name[j] << std::endl;
				j++;
			}
		}
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
		std::cout << "Max Body Size : " << conf[i].max_body_size << "\n";
		std::cout << "Root : " << conf[i].root << std::endl;
		std::cout << "Index : " << conf[i].index << std::endl;
		std::cout << "-------------------" << std::endl;
		{
			int j = 0;
			while (j < conf[i].location.size())
			{
				std::cout << "pattern : " << conf[i].location[j].pattern << std::endl;
				std::cout << "root : " << conf[i].location[j].root << std::endl;
				std::cout << "index : " << conf[i].location[j].index << std::endl;
				std::cout << "autoindex : " << conf[i].location[j].autoindex << std::endl;
				std::cout << "upload : " << conf[i].location[j].upload << std::endl;
				std::cout << "upload_store : " << conf[i].location[j].upload_store << std::endl;
				std::cout << "cgi : " << conf[i].location[j].cgi_ext << std::endl;
				std::cout << "cgi_path : " << conf[i].location[j].cgi_path << std::endl;
				int k = 0;
				while (k < conf[i].location[j].error_page.size())
				{
					std::cout << "error code : " << conf[i].location[j].error_page[k].first << std::endl; 
					std::cout << "error file : " << conf[i].location[j].error_page[k].second << std::endl; 
					k++;
				}
				k = 0;
				while (k < conf[i].location[j].allow.size())
				{
					std::cout << "allow : " << conf[i].location[j].allow[k] << "\n";
					k++;
				}
				j++;
			}
		}
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


void	getPairValue(std::vector<std::pair<std::string, std::string> >& holder, std::string value)
{
	int i;
	int j;
	int size;
	std::pair<std::string , std::string> pr;
	size = value.size();
	while (i < size && (value[i] == ' ' || value[i] == '\t'))
		i++;
	j = 0;
	while (i + j < size && value[i + j] != ' ' && value[i + j] != '\t')
		j++;
	if (j)
		pr.first.assign(value.substr(i, j));
	i = i + j;
	while (i < size && (value[i] == ' ' || value[i] == '\t'))
		i++;
	j = 0;
	while (i + j < size && value[i + j] != ' ' && value[i + j] != '\t')
		j++;
	if (j)
		pr.second.assign( value.substr(i, j));
	holder.push_back(pr);
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
			conf.listen.push_back(value);
		}
		if (key == "servername")
		{
			getMultivalue(conf.server_name, value);
		}
		if (key == "error_page")
		{
			getPairValue(conf.error_page, value);
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
		if (key == "location")
		{
			struct location loc;
			while (i < size && isBracket(serv[i]) != 2)
			{
				if (key == "location")
					loc.pattern.assign(value);
				if (key == "root")
					loc.root.assign(value);
				if (key == "cgi")
					loc.cgi_ext.assign(value);
				if (key == "script_filename")
					loc.cgi_path.assign(value);
				if (key == "upload")
					loc.upload.assign(value);
				if (key == "upload_store")
					loc.upload_store.assign(value);
				if (key == "error_page")
					getPairValue(loc.error_page, value);
				if (key == "index")
					loc.index.assign(value);
				if (key == "autoindex")
					loc.autoindex.assign(value);
				if (key == "allow")
					getMultivalue(loc.allow, value);
				i++;
				key.assign("");
				value.assign("");
				getKeyandValue(key, value, serv[i]);
			}
			conf.location.push_back(loc);
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
