/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getConfigData.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/19 17:44:50 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/03/29 01:06:05 by tel-bouh         ###   ########.fr       */
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
	if (j >= i)
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
				{
					std::cout << "val : {" << value << "}" << std::endl;
					loc.pattern.assign(value);
				}
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
}
