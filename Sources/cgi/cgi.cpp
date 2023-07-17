/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/19 16:05:26 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/17 18:38:10 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

void	generate_CGI_file(struct client &clt,std::string &filePath)
{
	if (clt.cgi.extention == ".php")
	{
		filePath = parsePHPcgi(clt.cgi.outFile, clt.cgi.header, intToString(clt.fd));
		std::cout << PURPLE << filePath << std::endl << END ;
	}
	else if (clt.cgi.extention == ".py" && !clt.cgi.loop_detected)
		filePath = "out" + intToString(clt.fd) +".html";
	else if (clt.cgi.extention == ".py" && clt.cgi.loop_detected)
	{
		kill(clt.cgi.pid, SIGKILL);
		error(clt, 508);
		return ;
	}
	clt.response.statusCode = 200;
}

void	executeCgi(struct client &clt,CGI &cgi, std::string &filePath)
{
	cgi.outFile = "out" +intToString(clt.fd) + ".html";
	int fd_out = open(cgi.outFile.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd_out == -1)
		throw std::runtime_error("Error1: opening file\n");
	// if (!cgi.loop_detected)
	// {
		if ((cgi.pid = fork()) == -1)
			throw std::runtime_error("Error: fork\n");
		if (!cgi.pid)
		{
			dup2(fd_out, STDOUT_FILENO);
			close(fd_out);

			const char* arg[] = {cgi.interpreter.c_str() , filePath.c_str(), NULL};
			char** env = new char*[cgi.env.size() + 2];  //!
			size_t i;
			for (i = 0; i < cgi.env.size(); i++)
				env[i] = const_cast<char*>(cgi.env[i].c_str());
			env[i++] = const_cast<char*>("REDIRECT_STATUS=1");  //! for php
			env[i] = NULL;

			//TODO after post is ready
			//TODOif(clt.map_request["Method"] == "POST")
			//TODO{
			//TODO	std::fstream in("in");
			//TODO	in << clt.map_request["QUERY_STRING"];
			//TODO	in.close();
			//TODO	// int fd = open("in", O_CREAT | O_TRUNC | O_RDWR, 0644);
			//TODO	// if (fd < 0)
			//TODO	// 	throw std::runtime_error("Error: failed to open input file");
			//TODO	// dup2(fd, 0);
			//TODO}
			if (execve(cgi.interpreter.c_str(), const_cast<char**>(arg), env) == -1)
			{
				std::cerr << "Error: execve\n";
				exit(0);
			}
		}
	
		sleep(1);
		int returnValue = waitpid(cgi.pid, NULL, WNOHANG);
		if (returnValue < 0)
			throw std::runtime_error("Error: wait failed");
		else if (returnValue)
		{
			generate_CGI_file(clt, filePath);
			return ;
		}
		else
		{
			cgi.loop_detected = true;
			cgi.time = 0;
			std::cout << YELLOW << "LOOP DETECTED in " << cgi.time << "for client " << clt.fd << END << std::endl;
			return;
		}
	// }
}


int cgi(struct webserv &web, struct client &clt)
{
	std::string extention;
	int			status;

	clt.response.cgi = true;
	if ((status = isCgiConfigured(clt, web, clt.map_request["URI"])) != 1)
		return status;
	fill_CGI_ENV(clt, web);
	try {
			executeCgi(clt,clt.cgi, clt.map_request["URI"]);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return error(clt, 500);
	}
	return clt.response.statusCode = 200;
}