/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 16:04:39 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/25 23:57:33 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"


void getResponseHeader(struct client &clt, int statusCode, std::string filePath)
{
	std::string response;

	clt.response.header = true;
	if (statusCode < 0)
		statusCode = statusCode * -1;
	response = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	response += "Transfer-Encoding: chunked\r\nServer: webserver-c\r\n";
	response += "Connection: keep-alive\r\n";
	response += "Content-Type: " + getContentType(filePath) + "\r\n";
	response += "Content-Length:" + intToString(clt.response.len) +"\r\n\r\n";
	clt.response.responseData.assign(response.begin(), response.end());
	clt.response.responseData.insert(clt.response.responseData.end(),
						clt.response.responseBody.begin(), clt.response.responseBody.end());
}

void	readFile(int statusCode, struct client &clt, std::string filePath)
{
	std::ifstream file;
	std::string chunkHeader;

	file.open(filePath.c_str(), std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "COULD NOT OPEN FILE\n";
		return ;//!404
	}
	if (clt.response.nbrFrames < 0)
		initData(clt, filePath, file);
	if (clt.response.nbrFrames == 0)
	{
		clt.response.finishReading = true;
		clt.response.sizeFrame = clt.response.fileSize;
	}
	else
	{
		clt.response.fileSize -= clt.response.sizeFrame;
		clt.response.nbrFrames--;
	}
	std::vector<char> buffer(clt.response.sizeFrame);

	file.seekg(clt.response.position);
	file.read(buffer.data(), clt.response.sizeFrame);
	clt.response.position = file.tellg();
	
	clt.response.responseBody.clear();

	chunkHeader = decimalToHex(clt.response.sizeFrame) + "\r\n" ;
	clt.response.responseBody.assign(chunkHeader.begin(), chunkHeader.end());
	clt.response.responseBody.insert(clt.response.responseBody.end(),
				buffer.begin(), buffer.end());
	chunkHeader.clear();
	chunkHeader = "\r\n";
	clt.response.responseBody.insert(clt.response.responseBody.end(),
			chunkHeader.begin(), chunkHeader.end());
	if (clt.response.finishReading)
	{
		chunkHeader.clear();
		chunkHeader = "0\r\n\r\n";
		clt.response.responseBody.insert(clt.response.responseBody.end()
			,chunkHeader.begin(), chunkHeader.end());
	}
	file.close();
}

void fillResponse(struct client &clt, struct webserv &web, int statusCode)
{
	if (!clt.response.header)
		clt.response.filePath = clt.map_request["URI"];
	
	readFile(statusCode, clt, clt.response.filePath);	
	if (!clt.response.header)
		getResponseHeader(clt, statusCode, clt.response.filePath);
	else
	{
		clt.response.responseData.clear();
		clt.response.responseData.assign(
				clt.response.responseBody.begin(), clt.response.responseBody.end());
	}
}



int sendResponse(struct client &clt, struct webserv &web, int statusCode)
{
	if (clt.response.error || clt.response.autoindex)
		fillErrorResponse(clt, web, statusCode);
	else if (!statusCode || statusCode >= 300)
		fillRedirectResponse(clt, web, statusCode);
	else
		fillResponse(clt, web, statusCode);

	if (send(clt.fd, clt.response.responseData.data(),
		clt.response.responseData.size(), 0) < 0)
	
	if (clt.response.finishReading)
		clt.response.position = 0;
	return statusCode;
}

