/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlerequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tel-bouh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 14:02:55 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/17 15:11:03 by tel-bouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

//char    temp[2048] = "HTTP/1.0 200 OK\r\n Server: webserver-c\r\n Content-type: text/html\r\n\r\n <html>hello, world    </html>\r\n";

int				hexIndex(char c)
{
	std::string			base = "0123456789abcdef";
	int					i;
	
	i = 0;
	while (i < 16)
	{
		if (i < 10 && c == base[i])
			return (i);
		if (i > 10 && (c == base[i] || c == (base[i] - 32)))
			   return (i);
		i++;
	}
	return (-1);
}

unsigned long	hexToDec(std::string hex)
{
	int 			i;
	unsigned long	rst;
	int				size;

	rst = 0;
	i = 0;
	size = hex.size();
	if (size == 0)
		return (0);
	while (i < size)
	{
		rst = rst + (hexIndex(hex[i]) * (int)pow(16, (size - i - 1)));
		i++;
	}
	return (rst);
}



int	toInt(std::string nbr)
{
	int i;
	int rst;
	int size;
	int	sign;

	i = 0;
	rst = 0;
	sign = 1;
	size = nbr.size();
	while (i < size)
	{
		rst = rst * 10;
		rst = rst + (nbr[i] - 48);
		i++;
	}
	return (rst * sign);
}



int	handleContinue(char	*line)
{
	std::string	str;
	int			rslt;

	str.assign(line);
	rslt = str.find("Expect: 100-continue");
	if (rslt != -1)
		return (0);
	return (1);
}

int	contentLength(std::string	buffer, unsigned long& content_len)
{
	int i;
	int j;
	int size;
	int find;
	
	size = buffer.size();
	if (content_len == 0)
	{
		find = buffer.find("Content-Length:");
		if (find > 0)
		{
			i = find + 15;
			while (i < size && buffer[i] == ' ')
				i++;
			j = 0;
			while (i + j < size && (buffer[i + j] >= '0' && buffer[i + j] <= '9'))
				j++;
			content_len = toInt(buffer.substr(i, j));
		}
		else
			content_len = -1;
	}
	if (content_len > 0)
	{
		find = buffer.find("\r\n\r\n") + 4;
		if (find > 0)
		{
			std::cout << "out " << buffer.size() << " " << (content_len + find) << std::endl;
			if (buffer.size() >= (content_len + find))
				return (1);
		}
	}
	return (0);
}

int		getBoundary(std::string buffer, std::string& boundary)
{
	int				i;
	int				size;
	int 			find;
	std::string		temp;

	find = buffer.find("Content-Type:");
	if (find == -1)
		return (-1);
	i = 0;
	size = buffer.size();
	while (find + i < size && buffer[find + i] != '\n')
		i++;
	temp.assign(buffer.substr(find, i));
	find = temp.find("boundary=");
	if (find == -1)
		return (-1);
	i = 0;
	find = find + 9;
	size = temp.size();
	while (find + i < size && (temp[find + i] != '\n' && temp[find + i] != ' ' && temp[find + i] != '\t'))
		i++;
	boundary = temp.substr(find, i - 1);
	boundary += "--";
	return (0);
}

void	getBodyType(std::string buffer, struct body& bodys)
{
	int				i;
	int				j;
	int				size;
	int				find;
	std::string		temp;
	std::string		hex;

	find = buffer.find("Content-Type:");
	if (find >= 0)
	{
		size = buffer.size();
		while (find + i < size && buffer[find + i] != '\n')
			i++;
		temp.assign(buffer.substr(find, i));
		find = temp.find("boundary=");
		if (find >= 0)
		{
			i = 0;
			find = find + 9;
			size = temp.size();
			while (find + i < size && (temp[find + i] != '\n' && temp[find + i] != ' ' && temp[find + i] != '\t'))
				i++;
			bodys.boundary = temp.substr(find, i - 1);
			bodys.boundary += "--";
			bodys.boundary_flag = 1;
			bodys.get_body_type = 1;
			return;
		}
	}
	find = buffer.find("Transfer-Encoding:");
	if (find >= 0)
	{
		i = find + 19;
		j = 0;
		while (i + j < size && buffer[i + j] != '\n')
			j++;
		temp = buffer.substr(i, j);
		find = temp.find("chunked");
		if (find >= 0)
		{
			find = buffer.find("Content-Length:");
			if (find >= 0)
			{
				size = buffer.size();
				i = find + 15;
				while (i < size && buffer[i] == ' ')
					i++;
				j = 0;
				while (i + j < size && (buffer[i + j] >= '0' && buffer[i + j] <= '9'))
					j++;
				bodys.chunks_con_len = toInt(buffer.substr(i, j));
				if (bodys.chunks_con_len > 0)
				{
					bodys.chunks_flag = 1;
					bodys.get_body_type = 1;
				}
			}
			find = buffer.find("\r\n\r\n");
			i = find + 4;
			j = 0;
			size = buffer.size();
			while (i < size && buffer[i] == ' ')
				i++;
			while (i + j < size && buffer[i + j] != '\r' && buffer[i + j] != ' ' && buffer[i + j] != '\t' && buffer[i + j] != '\n')
				j++;
			hex = buffer.substr(i, j);
			bodys.cr_index = find + 4 + hex.size() + 2;
			bodys.chunks_len = hexToDec(hex);
			if (bodys.chunks_len > 0)
			{
				bodys.chunks_flag = 1;
				bodys.n_chunks = 1;
				bodys.get_body_type = 1;
				return;
			}
		}
	}
	find = buffer.find("Content-Length:");
	if (find >= 0)
	{
		size = buffer.size();
		i = find + 15;
		while (i < size && buffer[i] == ' ')
			i++;
		j = 0;
		while (i + j < size && (buffer[i + j] >= '0' && buffer[i + j] <= '9'))
			j++;
		bodys.content_len = toInt(buffer.substr(i, j));
		if (bodys.content_len > 0)
			bodys.content_length_flag = 1;
		else
		{
			bodys.cr_nl_flag = 1;
			bodys.get_body_type = 1;
		}
		return ;	
	}
	else
	{
		bodys.cr_nl_flag = 1;
		bodys.get_body_type = 1;
	}

}


int	endOfChunks(std::string	buffer, struct body& bodys)
{
	int i;
	int j;
	int size;
	std::string	hex;
	i = 0;
	size = buffer.size();
	while (1)
	{
		if (size >= (bodys.cr_index + bodys.chunks_len))
		{
				i = bodys.cr_index + bodys.chunks_len;
				while (i < size && (buffer[i] == '\r' || buffer[i] == '\n'))
					i++;
				j = 0;
				while (i + j < size && buffer[i + j] != '\n' && buffer[i + j] != '\r')
					j++;
				if (j && buffer[i + j] == '\r')
				{
					hex = buffer.substr(i , j);
					bodys.chunks_len = hexToDec(hex);
					if (bodys.chunks_len == 0)
					{
						return (1);
					}
					else
					{
						i = i + j;
						while (i < size && (buffer[i] == '\r' || buffer[i] == '\n'))
							i++;
						bodys.cr_index = i;
					}
				}
				else
					break;
		}
		else
			break;
		if (buffer.size() > 6 && (buffer.substr(buffer.size() - 8, 7) == "0\r\n\r\n\r\n"))
		{
			return (1);
		}
	}
	return (0);
}




int	endOfTheRequest(std::string buffer, struct body& bodys)
{
	int find;
	int i;
	int len;
	
	if (bodys.get_body_type == 0)
		getBodyType(buffer, bodys);
	if (bodys.boundary_flag)
	{
		if (buffer.find(bodys.boundary) != -1)
			return (0);
	}
	if (bodys.chunks_flag)
	{
		if (endOfChunks(buffer, bodys))
			return (0);
	}
	if (bodys.content_length_flag)
	{
		find = buffer.find("\r\n\r\n") + 4;
		if (buffer.size() >= (bodys.content_len + find))
				return (0);
	}
	if (bodys.cr_nl_flag)
	{
		if (buffer.size() > 4 &&  buffer.compare(buffer.size() - 4, 4, "\r\n\r\n") == 0)
		{
			return (0);
		}
	}
	return (1);
}
