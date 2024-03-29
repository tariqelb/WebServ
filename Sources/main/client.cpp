/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 00:19:32 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/07/20 12:34:21 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"


uploadFiles::uploadFiles()
{
	len = 0;
	filename = "";
	file = new std::fstream();
}

uploadFiles::~uploadFiles()
{
	delete file;
}

uploadFiles::uploadFiles(const uploadFiles& rhs)
{
	len = rhs.len;
	filename = rhs.filename;
	file = new std::fstream();
}

uploadFiles&	uploadFiles::operator=(const uploadFiles& rhs)
{
	if (this != &rhs)
	{
		delete file;
		len = rhs.len;
		filename = rhs.filename;
		file = new std::fstream();
	}
	return (*this);
}

client::client()
{	
	config = -1;
	location = -1;
	len = 0;
	fd = -1;
	request_is_ready = false;
	response_is_ready = false;
	bodys.plan_hex = false;
	bodys.left_bytes = 0;
	bodys.get_body_type = 0;
	bodys.rd_bytes = 0;
	bodys.chunks_flag = 0;
	bodys.boundary_flag = 0;
	bodys.content_length_flag = 0;
	bodys.cr_nl_flag = 0;
	bodys.n_chunks = 0;
	bodys.cr_index = -1;
	bodys.chunks_len = 0;
	bodys.chunks_con_len = 0;
	bodys.content_len = 0;
	bodys.content_disposition = 0;
	bodys.boundary.assign("");
	headers.assign("");
	file = new std::fstream();
	file_name.assign("");
	body_data.assign("");
	nbr_of_reads = 0;
	post_flag = 0;
	body_data.assign("");
	body_length = 0;
	bodys.index = 0;
}

client::~client() 
{
	delete file;
}

client::client(const client& rhs)
{
	int				i;
	int				size;

	map_request = rhs.map_request;
	config = rhs.config;
	location = rhs.location;
	response = rhs.response;
	addr = rhs.addr;
	len = rhs.len;
	fd = rhs.fd;
	request_is_ready = rhs.request_is_ready;
	response_is_ready = rhs.response_is_ready;
	nbr_of_reads = rhs.nbr_of_reads;
	post_flag = rhs.post_flag;
	body_data = rhs.body_data;
	bodys.plan_hex = rhs.bodys.plan_hex;
	bodys.left_bytes = rhs.bodys.left_bytes;
	bodys.get_body_type = rhs.bodys.get_body_type;
	bodys.rd_bytes = rhs.bodys.rd_bytes;
	bodys.chunks_flag = rhs.bodys.chunks_flag;
	bodys.boundary_flag = rhs.bodys.boundary_flag;
	bodys.content_length_flag = rhs.bodys.content_length_flag;
	bodys.cr_nl_flag = rhs.bodys.cr_nl_flag;
	bodys.n_chunks = rhs.bodys.n_chunks;
	bodys.cr_index = rhs.bodys.cr_index;
	bodys.chunks_len = rhs.bodys.chunks_len;
	bodys.chunks_con_len = rhs.bodys.chunks_con_len;
	bodys.content_len = rhs.bodys.content_len;
	bodys.content_disposition = rhs.bodys.content_disposition;
	bodys.boundary = rhs.bodys.boundary;
	bodys.index = 0;
	headers = rhs.headers;
	file = new std::fstream();
	file_name = rhs.file_name;
	body_data = rhs.body_data;
	nbr_of_reads = rhs.nbr_of_reads;
	post_flag = rhs.post_flag;
	body_length = rhs.body_length;
	i = 0;
	size = rhs.upload_files.size();
	while (i < size)
	{
		upload_files.push_back(rhs.upload_files[i]);
		i++;
	}
	cgi = rhs.cgi;
}

client&		client::operator=(const client& rhs)
{
	int i;
	int size;

	if (this != &rhs)
	{
		map_request = rhs.map_request;
		config = rhs.config;
		location = rhs.location;
		response = rhs.response;
		addr = rhs.addr;
		len = rhs.len;
		fd = rhs.fd;
		request_is_ready = rhs.request_is_ready;
		response_is_ready = rhs.response_is_ready;
		nbr_of_reads = rhs.nbr_of_reads;
		post_flag = rhs.post_flag;
		body_data = rhs.body_data;
		bodys.plan_hex = rhs.bodys.plan_hex;
		bodys.left_bytes = rhs.bodys.left_bytes;
		bodys.get_body_type = rhs.bodys.get_body_type;
		bodys.rd_bytes = rhs.bodys.rd_bytes;
		bodys.chunks_flag = rhs.bodys.chunks_flag;
		bodys.boundary_flag = rhs.bodys.boundary_flag;
		bodys.content_length_flag = rhs.bodys.content_length_flag;
		bodys.cr_nl_flag = rhs.bodys.cr_nl_flag;
		bodys.n_chunks = rhs.bodys.n_chunks;
		bodys.cr_index = rhs.bodys.cr_index;
		bodys.chunks_len = rhs.bodys.chunks_len;
		bodys.chunks_con_len = rhs.bodys.chunks_con_len;
		bodys.content_len = rhs.bodys.content_len;
		bodys.content_disposition = rhs.bodys.content_disposition;
		bodys.boundary = rhs.bodys.boundary;
		bodys.index = 0;
		headers = rhs.headers;
		delete file;
		file = new std::fstream();
		file_name = rhs.file_name;
		body_data = rhs.body_data;
		nbr_of_reads = rhs.nbr_of_reads;
		post_flag = rhs.post_flag;
		body_length = rhs.body_length;
		i = 0;
		size = rhs.upload_files.size();
		while (i < size)
		{
			upload_files.push_back(rhs.upload_files[i]);
			i++;
		}
		cgi = rhs.cgi;
	}
	return (*this);
}

CGI::CGI() : header(""), loop_detected(false), time(0)
{
}

CGI::CGI(const CGI& rhs)
{
	cgi_ENV = rhs.cgi_ENV;
	interpreter = rhs.interpreter;
	env = rhs.env;
	header = rhs.header;
	extention = rhs.extention;
	loop_detected = rhs.loop_detected;
	time = rhs.time;
	pid = rhs.pid;
	outFile = rhs.outFile;
}

CGI&	CGI::operator=(const CGI& rhs)
{
	if (this != &rhs)
	{
		cgi_ENV = rhs.cgi_ENV;
		interpreter = rhs.interpreter;
		extention = rhs.extention;
		env = rhs.env;
		header = rhs.header;
		loop_detected = rhs.loop_detected;
		time = rhs.time;
		pid = rhs.pid;
		outFile = rhs.outFile;
	}
	return (*this);
}

CGI::~CGI()
{
}
