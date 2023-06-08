/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/08 19:46:44 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void getResponse(struct client &clt, int statusCode,
			std::string &response, std::string responseContent, std::string filePath)
{
	response = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	switch (statusCode)
	{

		case 302:
			response += "location: " + clt.map_request["URI"] + "\r\n\r\n";
			return;
		case 301:
			response += "Connection: close\r\nServer: webserver-c\r\n ";
			response +=  "Content-type: text/html\r\n\r\n";
			response += " <html> <h1>Moved Permanently => need to be handled<h1> </html>\r\n";
			return;
		case 0:
			response += "Connection: \r\nServer: webserver-c\r\n ";
			response +=  "Content-type: text/html\r\n\r\n";
			response += " <html> <h1> CGI or Post or delete or something not yet handled <h1>  </html>\r\n";
			return ;
	}
	response += "Connection: close\r\nServer: webserver-c\r\n ";
	response += "Content-Type: " + getContentType(filePath) + "\r\n";
	response += "Content-Length:" + intToString(responseContent.size()) +"\r\n\r\n";
	response += responseContent;
}

std::string readFileContent(std::string &filePath, int statusCode)
{
	std::string responseBody;
	std::fstream file;

	if (filePath == "Default")
		filePath = "www/error/"
					+ intToString(statusCode) + ".html";
	file.open(filePath.c_str(), std::ios::in);
	if (!file.is_open() && statusCode != 200 && statusCode != -302)
	{
		filePath = "www/error/"
					+ intToString(statusCode) + ".html";
		file.open(filePath.c_str(), std::ios::in);
	}
	getline(file, responseBody, '\0');
	return responseBody;
}

std::string getContentType(std::string filePath)
{;
	std::map<std::string, std::string> contentTypes;
	std::map<std::string, std::string>::iterator iter;
	std::string type;
	int index;
	
	std::cout << "file path = " << filePath << std::endl;
	fillMapContentTypes(contentTypes);
	index = filePath.rfind('.');
	type = filePath.substr(index, filePath.size());
	if ((iter = contentTypes.find(type)) != contentTypes.end())
		return iter->second;
	return "application/octet-stream";
}

void fillResponseHeader(struct client &clt, struct webserv &web, int statusCode)
{
	// std::cout << "status code = " << statusCode << std::endl;
	std::vector<std::pair<std::string, std::string> >::iterator iter;
	std::string response, fileBody, filePath;

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
	if (iter == web.config[clt.config].error_page.end())
		filePath = "default";
	if (statusCode == 200 || statusCode == -302)
		filePath = clt.map_request["URI"];
	if (statusCode != 301 && statusCode != 302)
		fileBody = readFileContent(filePath, statusCode);
	std::cout << "status code " << statusCode << "\n";
	getResponse(clt, statusCode, response, fileBody, filePath);
	// std::cout << "response  == " << response << "\n";
	// std::cout << "sendeeed " << send(clt.fd, response.c_str(), response.size(), 0) << "\n";
	
}


int sendResponse(struct client &clt, struct webserv &web, int statusCode)
{
	fillResponseHeader(clt, web, statusCode);
	return statusCode;
}
