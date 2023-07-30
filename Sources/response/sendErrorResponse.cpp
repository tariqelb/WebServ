/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendErrorResponse.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 12:01:11 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/29 23:29:24 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

std::string	generateErrorFile(struct client &clt, struct webserv &web, int statusCode)
{
	(void)web;
	std::string filePath("www/error/generated_"
					+ intToString(statusCode) + ".html");
	std::ofstream  errorFile(filePath.c_str());
	
	clt.response.generateError = true;
	if (!errorFile)
	{
		std::cerr << "error opening error file\n";
		return NULL;
	}

	errorFile	<< "<!DOCTYPE html>\n<html>\n<head>\n"
				<< "<title>" << getStatusMessage(statusCode)
				<< "</title>\n"
				<< "<style>\n"
				
				<< ".larger-text {\n"
				<< "font-size: 60px;\n"
				<< "text-align: center;\n}\n"
	
				<< "body {\n"
				<< "background-color: #080707;\n"
				<< "font-family: Courier New, monospace;\n}\n"
				<< "h1 {\n"
				<< "color: #fcfcfc;\n"
				<< "text-align: center;\n}"
		
				<<"p {\n"
				<< "text-align: center;\n}\n</style>\n"
				<< "/head>\n"
				<<"<body>\n"
				<< "<span class=\"larger-text\">"
				<< "\t<p>\n<h1><center>" 
				<< statusCode << ":\n" << getStatusMessage(statusCode)
				<< "</p></center></h1>"
				<< "</body>\n</html>";
	clt.map_request["URI"] = filePath;
	clt.response.remove = true;
	return filePath;
}


std::string getFilePath(struct client& clt, struct webserv &web, int statusCode)
{
	std::vector<std::pair<std::string, std::string> >::iterator iter;
	std::string filePath;
	struct stat pathStat;
	int status;

	if (clt.location >= 0 )
	{
		for (iter = web.config[clt.config].location[clt.location].error_page.begin();
		iter != web.config[clt.config].location[clt.location].error_page.end()
		&& iter->first != intToString(statusCode); iter++);
		if (iter != web.config[clt.config].location[clt.location].error_page.end())
			filePath = iter->second;
	}
	if (clt.location < 0
		|| iter == web.config[clt.config].location[clt.location].error_page.end())
	{
		for (iter = web.config[clt.config].error_page.begin();
			iter != web.config[clt.config].error_page.end()
			&& iter->first != intToString(statusCode); iter++);
		if (iter != web.config[clt.config].error_page.end())
			filePath = iter->second;
	}
	if (iter == web.config[clt.config].error_page.end()
		|| (iter != web.config[clt.config].error_page.end()
		&& ((status = stat(filePath.c_str(), &pathStat))
		|| (!status && !S_ISREG(pathStat.st_mode)))))
		filePath = generateErrorFile(clt, web, statusCode);
	return filePath;
}

void readeErrorFile(struct client &clt, int statusCode)
{
	std::ifstream file;
	std::stringstream responseBody;
	
	(void)statusCode;
	file.open(clt.response.filePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "ERROR : FAILED TO OPEN ERROR OR AUTOINDEX FILE \n";
		return;
	}
	responseBody << file.rdbuf();
	std::copy( std::istreambuf_iterator<char>(responseBody),
		std::istreambuf_iterator<char>(), std::back_inserter(clt.response.responseBody));
	clt.response.error = true;
}


void	getResponseHeaderError(struct webserv& web, struct client &clt, int statusCode)
{
	std::string response;

	response = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	response += "Connection: close\r\nServer: webserver-c\r\n";
	if (statusCode == 405 && clt.location >= 0)
	{
		response += "Allow: ";
		for(std::vector<std::string>::iterator iter = web.config[clt.config].location[clt.location].allow.begin();
			iter != web.config[clt.config].location[clt.location].allow.end();iter++)
		{
			if (iter != web.config[clt.config].location[clt.location].allow.begin())
				response += std::string(", ") + *iter;
			else
				response += *iter;
		}
		response += "\r\n";
	}
	response += "Content-Type: " + getContentType(clt.response.filePath) + "\r\n";
	response += "Content-Length:" + intToString(clt.response.responseBody.size()) +"\r\n\r\n";
	clt.response.responseData.assign(response.begin(), response.begin() + response.size());
	if (clt.map_request["Method"] != "HEAD")
		clt.response.responseData.insert(clt.response.responseData.end(),
						clt.response.responseBody.begin(), clt.response.responseBody.end());
}

void	fillErrorResponse(struct client &clt, struct webserv &web, int statusCode)
{
	if (clt.response.autoindex)
		clt.response.filePath = clt.map_request["URI"];
	else
		clt.response.filePath = getFilePath(clt, web, statusCode);
	readeErrorFile(clt, statusCode);
	getResponseHeaderError(web, clt, statusCode);
}