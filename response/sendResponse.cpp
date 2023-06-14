/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/14 13:22:34 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


void getResponseHeader(struct client &clt, int statusCode, std::string filePath)
{
	// std::string response;

	clt.response.header = true;
	clt.response.responseData = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	switch (statusCode)
	{

		case 302:
			clt.response.finishReading = true;
			clt.response.error = true;
			clt.response.responseData += "location: " + clt.map_request["URI"] + "\r\n\r\n";
			return;
		case 301:
			clt.response.finishReading = true;
			clt.response.error = true;
			clt.response.responseData += "Connection: close\r\nServer: webserver-c\r\n ";
			clt.response.responseData +=  "Content-type: text/html\r\n\r\n";
			clt.response.responseData += " <html> <h1>Moved Permanently => need to be handled<h1> </html>\r\n";
			return;
		case 0:
			std::cout << "i am here\n";
			clt.response.finishReading = true;
			clt.response.error = true;
			clt.response.responseData += "Connection: close\r\nServer: webserver-c\r\n ";
			clt.response.responseData +=  "Content-type: text/html\r\n\r\n";
			clt.response.responseData += " <html> <h1> CGI or Post or delete or something not yet handled <h1>  </html>\r\n";
			return ;
	}
	if (statusCode == 200)
		clt.response.responseData += "Connection: keep-alive\r\nServer: webserver-c\r\n ";
	else
		clt.response.responseData += "Connection: close\r\nServer: webserver-c\r\n ";
	clt.response.responseData += "Content-Type: " + getContentType(filePath) + "\r\n";
	clt.response.responseData += "Content-Length:" + intToString(clt.response.len) +"\r\n\r\n";
	clt.response.responseData.insert(clt.response.responseData.end(),
		clt.response.responseBody.begin(), clt.response.responseBody.end());
}

void	readFile(int statusCode, struct client &clt, std::string filePath)
{
	std::ifstream file;
	int				sizeFrame(1024);

	file.open(filePath.c_str(), std::ios::binary);
	if (clt.response.nbrFrames < 0)
	{
		file.seekg(0, std::ios::end);
		clt.response.fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		clt.response.len = clt.response.fileSize;
		clt.response.nbrFrames = static_cast<unsigned long>(clt.response.fileSize) / 1024;
		clt.response.position = file.tellg();
	}
	if (clt.response.nbrFrames == 0)
	{
		clt.response.finishReading = true;
		sizeFrame = clt.response.fileSize;
		std::cout << "FINISH READING\n";
	}
	else
	{
		clt.response.fileSize -= sizeFrame;
		clt.response.nbrFrames--;
	}
	std::vector<char> buffer(sizeFrame);

	file.seekg(clt.response.position);
	file.read(buffer.data(), sizeFrame);
	clt.response.position = file.tellg();
	
	clt.response.responseBody.clear();
	 clt.response.responseBody.assign(buffer.begin(), buffer.begin() + sizeFrame);
	file.close();
}

void fillResponse(struct client &clt, struct webserv &web, int statusCode)
{
	if (statusCode != 301 && statusCode != 302 && statusCode != 0)
		readFile(statusCode, clt, clt.response.filePath);
	if (!clt.response.header)
		getResponseHeader(clt, statusCode, clt.response.filePath);
	else
		clt.response.responseData = clt.response.responseBody.data();
}

//!read
int sendResponse(struct client &clt, struct webserv &web, int statusCode)
{
	if (!clt.response.header)
	clt.response.filePath = getFilePath(clt, web, statusCode);
	fillResponse(clt, web, statusCode);
	if (send(clt.fd, clt.response.responseData.data(),
		clt.response.responseData.size(), 0) < 0)
		std::cout << "ERROR: send\n";

	if (clt.response.finishReading)
	{
		std::cout << "the response is suppose to be finished\n";
		clt.response.position = 0;
	}
	return statusCode;
}

