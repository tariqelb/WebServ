/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sendRedirectResponse.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 11:30:12 by hasabir           #+#    #+#             */
/*   Updated: 2023/06/16 18:20:21 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../webserv.hpp"

void	fillRedirectResponse(struct client &clt, struct webserv &web, int statusCode)
{
	std::string	response;

	std::cout << statusCode << std::endl;
	clt.response.finishReading = true;
	response = "HTTP/1.1 " + intToString(statusCode) + " " + getStatusMessage(statusCode);
	if (statusCode && statusCode != 200)
	{
		clt.response.error = true;
		response += "location: " + clt.map_request["URI"] + "\r\n\r\n";
		clt.response.responseData.assign(response.begin(), response.begin() + response.size());
		return;
	}
	clt.response.error = true;
	response += "Connection: close\r\nServer: webserver-c\r\n ";
	response +=  "Content-type: text/html\r\n\r\n";
	response += " <html> <h1> CGI or Post or delete or something not yet handled <h1>  </html>\r\n";
	clt.response.responseData.assign(response.begin(), response.begin() + response.size());
	return ;
}