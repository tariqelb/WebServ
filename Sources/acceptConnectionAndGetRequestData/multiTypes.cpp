/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiTypes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 19:54:34 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/06/29 12:50:30 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"


void	getFileSize(struct uploadFiles& file)
{
	file.file->open(file.filename.c_str(), std::fstream::in | std::fstream::binary );

	if (file.file->is_open() == true)
	{
		file.file->seekg(0, std::fstream::end);  // Seek to the end of the file
        //std::streampos fileSize = file.file->tellg();  // Get the current position
		file.file->close();
	}
}
std::map<std::string, std::string>::iterator content_type_dexist2(struct client& clt,std::string pp)
{
	std::map<std::string, std::string>::iterator it = clt.map_request.begin();
	for(;it != clt.map_request.end();it++)
	{
		if(it->first.compare(pp) != 0)
			continue;
		else
			break;
	}
	if(it != clt.map_request.end())
		return (it);
	return (it);
}
void new_string_without_boundary2(std::string &buffer, struct client &clt)
{
	
	std::string CTTYPE = "Content-Type";
	//size_t j = 0;
	std::string temp_content_type;
	std::map<std::string,std::string> ContentTypes;
	if(content_type_dexist2(clt,CTTYPE) != clt.map_request.end())
	{
		temp_content_type = clt.map_request["Content-Type"];
		temp_content_type = temp_content_type.erase(0,30);
		buffer.erase(0,buffer.find(temp_content_type));
		buffer = buffer.substr(52,buffer.size());
	}
	else
	{
		error(clt,403);
	}
}
void new_string_without_last_boundary2(std::string &buffer,struct client &clt)
{
	std::string CTTYPE = "Content-Type";
	//size_t j = 0;
	std::string temp_content_type;
	std::map<std::string,std::string> ContentTypes;
	

	if(content_type_dexist2(clt,CTTYPE) != clt.map_request.end())
	{
		temp_content_type = clt.map_request["Content-Type"];
		temp_content_type = temp_content_type.erase(0,30);
		temp_content_type = temp_content_type + "--" ;
		//std::cout << temp_content_type<<std::endl;
		size_t find;
		find = buffer.find(temp_content_type,0);
		if(buffer[find - 1] == '-' && buffer[find - 2] == '-')
			find = find - 2;
		
		//std::cout << buffer;
		// std::cout<< find<<std::endl;
		if(find != std::string::npos)
			buffer = buffer.erase(find,buffer.size());
	}
	else
	{
		error(clt,403);
	}
}
void new_string_without_last_hexa2(std::string &buffer)
{
	std::string fill = "";
	for(size_t i = 0;i < buffer.size();i++)
	{
		fill += buffer[i];
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				break;
			}
		}
	}
	fill += '\n';
	buffer = fill;
}
void new_string_show2(std::string &buffer)
{
	size_t	i = 0, j = 0;
	for(;i < buffer.size();i++)
	{
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				//std::cout << buffer<<std::endl;
				buffer.erase(0,i + 1);
				//std::cout << buffer<<std::endl;
				j+= 1;
				if(j > 0)
					break;
			}
		}

	}
}
void new_string_without_first_hexa2(std::string &buffer)
{
	size_t	i = 0;
	std::string fill = "";
	for(;i < buffer.size();i++)
	{
		fill += buffer[i];
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				buffer[i] = '\n';
				break;
			}
		}
	}
	int find = buffer.find(fill.c_str(),0);
	std::cout << find<<std::endl;
	buffer = buffer.substr(fill.size(),buffer.size());
}
void new_string_without_first_and_last_v22(std::string &buffer)
{
	size_t	i = 0;
	for(;i < buffer.size();i++)
	{
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				if(buffer[i + 2] == '\r')
				{
					if(buffer[i + 3] == '\n')
					{
						buffer.erase(0,i + 3);
					}
				}
			}
		}

	}
}
void new_string_without_first_and_last2(std::string &buffer)
{
	size_t	i = 0;
	for(;i < buffer.size();i++)
	{
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				if(buffer[i + 2] == '\r')
				{
					if(buffer[i + 3] == '\n')
					{
						buffer[i + 3] = ' ';
					}
				}
			}
		}

	}
}
std::string extractFieldName2(const std::string& header) {
    std::string name;
    size_t nameStart = header.find("name=\"");
    if (nameStart != std::string::npos) {
        nameStart += 6; // Move to the first character after "name=\""
        size_t nameEnd = header.find("\"", nameStart);
        if (nameEnd != std::string::npos) {
            name = header.substr(nameStart, nameEnd - nameStart);
        }
    }
    return name;
}
std::string extractFieldFileName2(const std::string& header) {
    std::string name;
    size_t nameStart = header.find("ilename=\"");
    if (nameStart != std::string::npos) {
        nameStart += 9; // Move to the first character after "name=\""
        size_t nameEnd = header.find("\"", nameStart);
        if (nameEnd != std::string::npos) {
            name = header.substr(nameStart, nameEnd - nameStart);
        }
    }
    return name;
}
void new_string_erase_content_disposition2(std::string &buffer)
{
	//size_t	i = 0;
	//int found_n = 0;
	if(buffer.find("filename") == std::string::npos)
		buffer = buffer.erase(0,34);
	else
		buffer = buffer.erase(0,32);
	std::string name = extractFieldName2(buffer);
	size_t i = 6 + name.size() + 1;
	// std::cout << buffer<<std::endl;
	buffer = buffer.erase(0,i);
	// std::cout << buffer<<std::endl;
	if(buffer.find("ilename",0) != std::string::npos)
	{
		std::string ilename = extractFieldFileName2(buffer);
		i = 10 + ilename.size() + 3;
		buffer.erase(0, i);
		// std::cout << buffer<<std::endl;

	}
}
void new_string_erase_content_type2(std::string &buffer)
{
	size_t find = buffer.find("Content-Type");
	if(find != std::string::npos)
	{
		size_t find_n = buffer.find("\n",find);
		buffer.erase(find,find_n - find + 1);
	}
}
void new_string_buff_12(std::string &buffer)
{
	size_t	i = 0;
	for(;i < buffer.size();i++)
	{
		if(buffer[i] == '\r')
		{
			if(buffer[i + 1] == '\n')
			{
				buffer[i] = 'f';
				buffer[i + 1] = 'o';
				return;
			}
		}

	}
}

int		isADerective(std::string buffer, int find, int size)
{
	int find_name;
	int find_cr_nl;
	int find_filename;
	(void)size;
	find_cr_nl = buffer.find("\r\n", find);
	find_name = buffer.find("name=", find);
	find_filename = buffer.find("filename=", find);
	if (find_cr_nl != -1 && find_name != -1 && find_filename != -1 && find_name < find_cr_nl && find_filename < find_cr_nl)
		return (1);
	if (find_cr_nl != -1 && find_name != -1  && find_name < find_cr_nl)
		return (1);
	return (0);

}

void	getFilename(std::string buffer, int file_index, struct uploadFiles* upload_files, int find, int fd,struct client &clt)
{
	int 			find_filename;
	int 			i;
	int				size;
	std::string		temp;
	int 			find_cr;
	(void)fd;
	find_filename = -1;
	size = buffer.size();
	find_cr = buffer.find("\r\n", find);
	temp = clt.temp_header;
	// std::cout << clt.temp_header<<std::endl;
	if (size && find_cr != -1)
	{

		temp = buffer.substr(find, find_cr - find);
		find_filename = temp.find("filename=");
		if (find_filename != -1)
			find_filename = buffer.find("filename=", find);
		upload_files->no_name = "no";
	}
	else if (size)
		find_filename = buffer.find("filename=", find);
	upload_files->no_name = "no";
	if (find != -1 && find_filename != -1)
	{
		find_filename += 10;
		i = 0;
		while (find_filename + i < size && buffer[find_filename + i] != '\"')
			i++;
		if (i)
		{
			temp = buffer.substr(find_filename, i);
			find_filename = temp.find_last_of("/");
			if (find_filename != -1)
			{
				temp = temp.substr(find_filename + 1);
				if (temp.size() == 0)
					temp =  "text_" + std::to_string(file_index) + ".txt";
			}
		}
		else
			temp = "text_" + std::to_string(file_index) + ".txt";
		upload_files->filename = "./www/uploads/" + temp;
		upload_files->just_the_file = temp;
		upload_files->no_name = "no";
	}
	else if(clt.temp_header[0] != 0) {
		temp = clt.temp_header;
		size_t content_di = temp.find("Content-Disposition");
		(void)content_di;
		find_filename = temp.find("filename=");
		find_filename += 9;
		int j = 0;
		while ((size_t)find_filename + j < temp.size() && temp[find_filename + j] != '\"')
			j++;
		temp = temp.substr(find_filename,j);
		upload_files->filename = "./www/uploads/" + temp;
		upload_files->just_the_file = temp;
		upload_files->no_name = "no";

	}
	else 
	{
		std::srand(static_cast<unsigned>(std::time(nullptr)));
		int a = std::rand();
		char r[1000000];
		std::sprintf(r, "%d", a);
		temp = "text_" + std::string(r) +std::to_string(file_index) + ".txt";
		upload_files->filename = "./www/uploads/" + temp;
		upload_files->just_the_file = temp;
		upload_files->no_name = "yes";
	}
}


void	multiTypes(std::string buffer,std::string &buffer2, struct client& clt)
{
	int						find_cr;
	int						find;
	int						size;
	int						i;
	int						flag_postman_curl;
	std::string				hex;
	std::string				temp;
	struct  uploadFiles     upload_files;
	std::string				theEnd;

	theEnd = "\r\n\r\n" + clt.bodys.boundary;
	flag_postman_curl = 1;
	size = buffer.size();
	i = 0;
	if (clt.bodys.content_length_flag == 1 && clt.bodys.chunks_flag == 0 && clt.bodys.boundary_flag == 0)
	{
		find = 0;
		if (clt.nbr_of_reads == 1)
		{
			// std::cout << BLUE<<clt.map_request["Content-Disposition"]<<std::endl<<END;
			find_cr = buffer.find("\r\n\r\n");
			if(find_cr != -1)
				clt.temp_header =  buffer.substr(0,find_cr + 4);
			// std::cout << clt.temp_header<< std::endl;
			if (find_cr != -1)
			{
				buffer = buffer.substr(find_cr + 4);
				buffer2 = buffer;
			}
		}
		i = clt.upload_files.size();
		if (buffer.size() && i == 0)
		{
			//std::cout << clt.nbr_of_reads<<std::endl;
			size_t founder_name = clt.temp_header.find("Content-Disposition");
			if(founder_name != std::string::npos)
			{	
				size_t find_r_n = clt.temp_header.find("\n",founder_name);
				clt.temp_header = clt.temp_header.substr(founder_name,find_r_n - founder_name);
			}
			else
			{
				clt.temp_header = "";
			}
			getFilename(buffer, clt.upload_files.size(), &upload_files, -1, clt.fd,clt);
			upload_files.file->open(upload_files.filename.c_str(),std::fstream::app | std::fstream::out);
			//sleep(2);
			if (upload_files.file->is_open() == true)
			{
				upload_files.file->close();
				clt.upload_files.push_back(upload_files);
				clt.upload_files[0].no_name = upload_files.no_name;
			}
		}
		i = clt.upload_files.size();
		if (i)
		{
			temp.assign("");
			temp = clt.upload_files[i - 1].filename;
			clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::app | std::fstream::out);
			if (buffer.size() >= clt.bodys.content_len)
				clt.upload_files[i - 1].file->write(buffer.c_str(), clt.bodys.content_len);
			else
			{
				clt.upload_files[i - 1].file->write(buffer.c_str(), buffer.size());
			}
			clt.upload_files[i - 1].file->close();
		}
	}
	else if (clt.bodys.boundary_flag == 1 || clt.bodys.chunks_flag == 1)
	{
		int 			flag;
		std::string		bndry;
		std::string		tmp;

		flag = 1;
		bndry = clt.bodys.boundary.substr(0, clt.bodys.boundary.size() - 2);
		// std::cout << buffer<<"    ll"<< std::endl;
		while (buffer.size())
		{
			find = 0;

			if (clt.nbr_of_reads == 1 && flag)
			{
				find_cr = buffer.find("\r\n\r\n");
				find = buffer.find(bndry, find_cr);
				if (find != -1)
				{
					buffer = buffer.substr(find);
					buffer2 = buffer;
					//std::cout << buffer<<std::endl;
				}
				flag = 0;
				// new_string_without_boundary2(buffer,clt);
				// new_string_without_last_boundary2(buffer,clt);
				// new_string_erase_content_type2(buffer);
				// new_string_erase_content_disposition2(buffer);
				// new_string_without_first_and_last_v22(buffer);
				//new_string_show2(buffer);
				//std::cout <<buffer;
				//exit(0);
			}
			else
			{
				find = buffer.find(bndry);
				if ((find >= 0 && find <= 4) ||  (clt.bodys.content_disposition == 1 && buffer.size()))
				{
					find = buffer.find("Content-Disposition:");
					if (find != -1 && isADerective(buffer, find, size) == 1 && clt.bodys.content_disposition == 0)
					{
						//std::cout << "Enter 1"<<std::endl;
						getFilename(buffer, clt.upload_files.size(), &upload_files, find, clt.fd,clt);
						upload_files.file->open(upload_files.filename.c_str(), std::fstream::app | std::fstream::out);
						if (upload_files.file->is_open() == true)
						{
							upload_files.file->close();
							clt.upload_files.push_back(upload_files);
							clt.upload_files[0].no_name = upload_files.no_name;
						}
						find_cr = buffer.find("\r\n\r\n", find);
						buffer = buffer.substr(find_cr + 4);
						clt.bodys.content_disposition = 1;
					}
					else if (clt.bodys.content_disposition == 0)
					{
						//std::cout << "Enter 2"<<std::endl;
						if (clt.upload_files.size() == 0)
						{
							getFilename(buffer, clt.upload_files.size(), &upload_files, find, clt.fd,clt);
							upload_files.file->open(upload_files.filename.c_str(), std::fstream::app | std::fstream::out);
							if (upload_files.file->is_open() == true)
							{
								upload_files.file->close();
								clt.upload_files.push_back(upload_files);
								clt.upload_files[0].no_name = upload_files.no_name;
							}
							clt.bodys.content_disposition = 1;
						}
						else 
							clt.bodys.content_disposition = 1;
					}
					if (clt.bodys.content_disposition == 1)
					{
						//std::cout << "Enter 5"<<std::endl;
						find = buffer.find(bndry);
						if (find == -1) // check for npos
						{
							find = buffer.find(clt.bodys.boundary);
						}
						i = clt.upload_files.size();
						if (find != -1 && i)
						{
							if (find > 1)
								find = find - 2;
							temp.assign("");
							temp = clt.upload_files[i - 1].filename;
							clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::app | std::fstream::out);
							temp = buffer.substr(0, find);
							clt.upload_files[i - 1].file->write(temp.c_str(), temp.size());
							clt.upload_files[i - 1].file->close();
								buffer = buffer.substr(find + 2);
							clt.bodys.content_disposition = 0;
						}
						else if (buffer.size() && i)
						{
							//std::cout << "Enter 6"<<std::endl;
							temp.assign("");
							temp = clt.upload_files[i - 1].filename;
							clt.upload_files[i - 1].file->open(temp.c_str(), std::fstream::app | std::fstream::out);
							temp = buffer.substr(0, buffer.size());
							clt.upload_files[i - 1].file->write(temp.c_str(), temp.size());
							clt.upload_files[i - 1].file->close();
							buffer = buffer.assign("");
							clt.bodys.content_disposition = 1;
						}
					}
					i = buffer.find(clt.bodys.boundary);
					if (i >= 0 && i < 3)
					{
						//std::cout << "Enter 7"<<std::endl;
						buffer.assign("");
					}
				}
			}
		}
	}
}
