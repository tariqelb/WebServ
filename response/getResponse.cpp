/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getResponse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:49:06 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/26 23:15:00 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


int autoindex(struct client& clt, struct webserv &web)
{
	std::ofstream autoindex((clt.map_request["URI"] + "autoindex.html").c_str());
	DIR* directory;
	struct dirent* en;
	std::string pattern;

	if (!autoindex)
	{
		std::cerr << "Failed to create autoindex.html" << std::endl;
		return error(clt, 500);
	}

	clt.response.autoindex = true;
	autoindex << "<html>\n"
				<< "<head><title>Index of " << clt.map_request["URI"] << "</title></head>\n"
				<< "<body>\n"
				<< "<h1>Index of " << clt.map_request["URI"] << "</h1>\n"
				<< "<hr>\n"
				<< "<ul>\n";

	directory = opendir(clt.map_request["URI"].c_str());
	if (directory)
	{
		if (*clt.response.uri.rbegin() != '/')
			clt.response.uri += "/";
		while ((en = readdir(directory)) != NULL)
		{
			if (!strcmp(en->d_name, "autoindex.html"))
				continue;
			autoindex	<< "<li>" << "<a href=\""
						<< "http://"
						<< clt.map_request["Host"] << clt.response.uri << en->d_name
						<< "\">"
						<< en->d_name << "</a></li>\n";
		}
		closedir(directory);
	}

	autoindex << "</ul>\n"
				<< "<hr>\n"
				<< "</body>\n"
				<< "</html>\n";

	autoindex.close();

	clt.map_request["URI"] +=  "autoindex.html";
	if (!clt.response.body)
		clt.response.statusCode = 200;
	return 0;
}

int	get(struct webserv& web, struct client& clt)
{
	struct stat pathStat;
	std::string path;

	if (stat(clt.map_request["URI"].c_str(), &pathStat))
	{
		if (clt.location >= 0
			&& !web.config[clt.config].location[clt.location].redirect.empty())
		{
			if (clt.response.statusCode)
				return clt.response.statusCode;
			return clt.response.statusCode = 302;
		}
		return error(clt, 404);
	} 
	if (!S_ISDIR(pathStat.st_mode))
	{
		if (clt.location >= 0
			&& !web.config[clt.config].location[clt.location].redirect.empty())
		{
			if (clt.response.statusCode)
				return clt.response.statusCode *= -1;
			clt.response.body = true;
			return clt.response.statusCode = 302;
		}
		if (clt.location >= 0 && !web.config[clt.config].location[clt.location].cgi.empty())
		{
			if (cgi(web, clt) == 200)
				return 200;
		}
		return clt.response.statusCode = 200;
	}
	if (*clt.map_request["URI"].rbegin() != '/')
	{
		clt.map_request["URI"] += "/";
		std::cout << "I AM SUPPOSE TO BE HERE\n";
		clt.response.body = true;
		clt.response.statusCode = 301;
	}
	if (clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
	{
		path = clt.map_request["URI"] + web.config[clt.config].location[clt.location].index;
	}
	else if (clt.location < 0 && !web.config[clt.config].index.empty())
		path = clt.map_request["URI"] + web.config[clt.config].index;
	if (stat(path.c_str(), &pathStat))
		path = clt.map_request["URI"] + "index.html";

	if (!stat(path.c_str(), &pathStat))
	{
		clt.map_request["URI"] = path;
		if (!clt.response.body)
			clt.response.statusCode = 200;
		return 0;
	}
	if (clt.location >= 0)
	{
		if (web.config[clt.config].location[clt.location].autoindex.empty() ||
			web.config[clt.config].location[clt.location].autoindex == "off")
			return error(clt, 403);
	}
	return autoindex(clt, web);
	return clt.response.statusCode = 0;
}