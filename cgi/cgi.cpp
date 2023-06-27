/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:05:26 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/27 16:46:16 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"
#include <unistd.h>

// void	fill_CGI_ENV(struct client &clt, struct webserv &web)
// {
// 	clt.cgi.env.push_back("QUERY_STRING=" + clt.map_request["QUERY_STRING"]);
// 	clt.cgi.env.push_back(std::string("REMOTE_ADDR=" + web.config[clt.config].host));
// 	clt.cgi.env.push_back(std::string("REQUEST_METHOD=GET"));
// 	clt.cgi.env.push_back(std::string("SCRIPT_NAME=") + clt.map_request["URI"]);
// 	clt.cgi.env.push_back("SERVER_PORT=" + clt.port);
// 	clt.cgi.env.push_back(std::string("SCRIPT_FILENAME=") + clt.map_request["URI"]);
// 	clt.cgi.env.push_back("HTTP_HOST="+clt.map_request["Host"]);
// 	clt.cgi.env.push_back("HTTP_USER_AGENT=" + clt.map_request["User-Agent"]);
// }

// void	executeCgi(CGI &cgi, std::string &filePath)
// {
// 	int fd[2];
// 	pid_t pid;
// 	if (pipe(fd))
// 	{
// 		std::cerr << "Error : pipe\n";
// 		return ;
// 	}

// 	if ((pid = fork()) == -1)
// 	{
// 		std::cerr << "Error : fork\n";
// 		return ;
// 	}
// 	if (!pid)
// 	{
// 		if (close (fd[0]) == -1)
// 			std::cerr << "Error closing fd[0]\n";
// 		dup2(fd[1], STDOUT_FILENO);
// 		close(fd[1]);
		
// 		const char* arg[] = {filePath.c_str(), NULL};
// 		char **env = new char *[cgi.env.size() + 1];
// 		int i;
// 		for (i = 0; i < cgi.env.size(); i++)
// 			env[i] = const_cast<char*>(cgi.env[i].c_str());
// 		env[i] = NULL;
// 		if (execve(filePath.c_str(), const_cast<char**>(arg), env) == -1)
// 		{
// 			std::cerr << "Error: execve\n";
// 			exit(0);
// 		}
// 	}
// 	waitpid(pid, 0, 0);
// 	std::fstream out;
// 	out.open("out.html", std::ios::out);
// 	if (!out.is_open())
// 	{
// 		std::cerr << "Error: out file\n";
// 		return ;
// 	}
// 	ssize_t bytesRead;
// 	struct stat fileState;
// 	fstat(fd[0], &fileState);
// 	std::cout << "fileState.st_size = " << fileState.st_size << std::endl;
// 	std::vector<char> buffer(static_cast<int>(fileState.st_size));
// 	if ((bytesRead = read(fd[0], buffer.data(), buffer.size())) > 0)
// 		out << buffer.data();
// 	else if  (bytesRead == -1) 
// 	{
// 		std::cerr << "Error reading from fd[0]\n";
// 		return;
// 	}
// 	close(fd[0]);
// 	out.close();
// 	filePath = "out.html";
// 	out.close();
// 	close (fd[1]);
// }

// int isCgiConfigured(struct client &clt, struct webserv &web, std::string &extention)
// {
// 	std::vector<std::pair<std::string, std::string> >::iterator iter;
// 	for (iter = web.config[clt.config].location[clt.location].cgi.begin();
// 		iter != web.config[clt.config].location[clt.location].cgi.end()
// 		&& iter->second != clt.map_request["URI"];
// 		iter++);
// 	if (iter == web.config[clt.config].location[clt.location].cgi.end())
// 	{
// 		std::cout << "I did not find any thing\n";
// 		return 0;
// 	}
// 	extention = iter->first;
// 	return 1;
// }



int cgi(struct webserv &web, struct client &clt)
{
	// std::string extention;

	// if (!isCgiConfigured(clt, web, extention))
	// 	return 0;
	// fill_CGI_ENV(clt, web);
	// executeCgi(clt.cgi, clt.map_request["URI"]);
	return clt.response.statusCode = 0;
}