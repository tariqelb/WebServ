/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:05:26 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/23 11:01:13 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"
#include <unistd.h>

void	fill_CGI_ENV(struct client &clt, struct webserv &web)
{
	clt.cgi.env.push_back("QUERY_STRING=" + clt.map_request["QUERY_STRING"]);
	clt.cgi.env.push_back(std::string("REMOTE_ADDR=" + web.config[clt.config].host));
	std::cout << "host = " << std::string("REMOTE_ADDR=" + web.config[clt.config].host) << std::endl;
	clt.cgi.env.push_back(std::string("REQUEST_METHOD=GET"));
	clt.cgi.env.push_back(std::string("SCRIPT_NAME=") + clt.map_request["URI"]);
	clt.cgi.env.push_back("SERVER_PORT=" + clt.port);
	clt.cgi.env.push_back(std::string("SCRIPT_FILENAME=") + clt.map_request["URI"]);
	clt.cgi.env.push_back("HTTP_HOST="+clt.map_request["Host"]);
	clt.cgi.env.push_back("HTTP_USER_AGENT=" + clt.map_request["User-Agent"]);

	for (int i = 0; i < clt.cgi.env.size(); i++)
		std::cout << clt.cgi.env[i] << std::endl;
}

void	executeCgi(CGI &cgi, std::string &filePath)
{
	int fd[2];
	std::cout << "I entered this function\n";
	if (pipe(fd))
	{
		std::cerr << "Error : pipe\n";
		return ;
	}
	pid_t pid;

	if ((pid = fork()) == -1)
	{
		std::cerr << "Error : fork\n";
		return ;
	}
	if (!pid)
	{
		if (close (fd[0]) == -1)
			std::cerr << "Error closing fd[0]\n";
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		
		const char* arg[] = {filePath.c_str(), NULL};
		char **env = new char *[cgi.env.size() + 1];
		int i;
		for (i = 0; i < cgi.env.size(); i++)
			env[i] = const_cast<char*>(cgi.env[i].c_str());
		env[i] = NULL;
		if (execve(filePath.c_str(), const_cast<char**>(arg), env) == -1)
		{
			std::cerr << "Error: execve\n";
			exit(0);
		}
	}
	std::fstream out;
	out.open("out.html", std::ios::out);
	if (!out.is_open())
	{
		std::cerr << "Error: out file\n";
		return ;
	}
	char buffer[100];
	ssize_t bytesRead;

	while ((bytesRead = read(fd[0], buffer, 1000)) > 0) {
		out.write(buffer, bytesRead);
	}

	if (bytesRead == -1) {
		std::cerr << "Error reading from fd[0]\n";
		return;
	}
	close(fd[0]);
	out.close();
	// std::cout << "I am in the parent process \n";
	// out << fd[0];
	filePath = "out.html";
	out.close();
	close (fd[1]);
	// close (fd[0]);
}

int cgi(struct webserv &web, struct client &clt)
{
	// std::vector<std::pair<std::string, std::string> >::iterator iter;
	// iter = web.config[clt.config].location[clt.location].cgi.begin();
	// std::cout << "key = " << iter->first << " | value = " << iter->second << std::endl;
	// fill_CGI_ENV(clt, web);
	// std::cout << "----------------\n";
	// executeCgi(clt.cgi, clt.map_request["URI"]);
	return clt.response.statusCode = 0;
}