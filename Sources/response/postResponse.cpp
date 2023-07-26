/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   postResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:48:15 by hasabir           #+#    #+#             */
/*   Updated: 2023/07/22 22:57:33 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../webserv.hpp"

std::string getContentTyp(std::string filePath)
{
	std::map<std::string, std::string> contentTypes;
	std::map<std::string, std::string>::iterator iter;
	std::string type;
	int index;
	
	fillMapContentTypes(contentTypes);
	index = filePath.find_last_of('.');
	if ((size_t)index == std::string::npos)
		return "application/octet-stream";
	type = filePath.substr(index, filePath.size());
	if ((iter = contentTypes.find(type)) != contentTypes.end())
		return iter->second;
	return "Not found";
}
unsigned long stringToUnsignedLong(const std::string& str) {
    unsigned long result = 0;

    for (size_t i = 0; i < str.length(); ++i) {
        char ch = str[i];
        if (ch >= '0' && ch <= '9') {
            result = result * 10 + static_cast<unsigned long>(ch - '0');
        } else {
            std::cerr << "Error: Invalid input string \"" << str << "\"\n";
            exit(1); // Or handle the error in your desired way
        }
    }

    return result;
}

int	post(struct webserv& web, struct client& clt)
{

	std::string path;
	std::string path2;
	std::string path3;

	struct stat pathStat1;
	struct stat pathStat2;
	struct stat pathStat3;

	if(clt.upload_files.size() <= 0)
		return error(clt,404);
	int place = web.config[clt.config].location[clt.location].upload_store.size() - 1;
	uploadFiles file = clt.upload_files[0];

	std::string temp22 = clt.upload_files[0].filename;
	std::string temp33 = temp22;
	std::string extension;
	std::ifstream file2;
	std::string comparitor = "no";
	std::string temp_str2;

	if(clt.bodys.chunks_flag == 1)
	{
		if(clt.body_length >  stringToUnsignedLong(web.config[clt.config].max_body_size))
			return (error(clt,413));
	}
	if(clt.upload_files[0].no_name.compare(comparitor.c_str()) == 0)
	{
		std::cout <<"Hereee 2"<<std::endl;
		extension = temp22.erase(0,14);
		bool only_one = false;
		std::string temp_original = temp33;
		size_t find_last_folder = clt.map_request["URI"].rfind("/");
		temp33 = temp33.erase(find_last_folder,temp33.size() - find_last_folder);
		if(temp33.rfind("/") == std::string::npos)
		{
			only_one = true;
			temp33 = temp_original + '/' + extension;
		}
		if(only_one == false)
			temp33 = temp33 + '/' + extension;
		file2.open(temp33,std::ios::in);
		file2.close();
		clt.upload_files[0].filename = extension;
		temp_str2 = clt.upload_files[0].filename;
		std::cout << temp33<<std::endl;
	}	
	else
	{
		
		temp_str2 =  clt.upload_files[0].filename;
		std::cout << "here"<<std::endl;
		temp22 = clt.map_request["URI"];
		std::cout << temp22<<std::endl;
		size_t find_last_fold = temp22.rfind("/");
		extension = temp22.erase(0,find_last_fold + 1);
		std::cout << extension<<std::endl;
		//temp22 = clt.map_request["URI"] + '/' + extension;
		std::ifstream file3;
		file3.open(clt.map_request["URI"],std::ios::in);
		file3.close();
		extension = clt.upload_files[0].filename.erase(0,14);
		clt.upload_files[0].filename = clt.map_request["URI"];
	}
	std::cout << clt.upload_files[0].just_the_file<< std::endl;
	if(stat(web.config[clt.config].location[clt.location].upload_store.c_str(),&pathStat1) == 0)
	{	
		if(clt.location >= 0 && web.config[clt.config].location[clt.location].upload == "on")
		{
			std::cout << "here 493"<<std::endl;
			// if(stat(clt.map_request["URI"].c_str(),&pathStat3) != 0)
			// 	return (error(clt,404));
			// if(S_ISDIR(pathStat3.st_mode))
			// 	return (error(clt,404));
			// std::ifstream original_file;
			// std::cout << "the file is "<< clt.map_request["URI"]<<std::endl;
			// original_file.open(clt.upload_files[0].filename.c_str(),std::ios::in);
			// if(original_file.is_open() == 0)
			// {
			// 	std::cerr<< "Error Opening the file\n"<<std::endl;
			// 	return  error(clt, 500);
			// }
			// original_file.close();
			if(access(web.config[clt.config].location[clt.location].upload_store.c_str(),X_OK) == 0)
			{
				
				std::string temp;
				std::ifstream input;
				std::ofstream out;
				while (web.config[clt.config].location[clt.location].upload_store[place] == '/' && place != 1)
				{
					temp = web.config[clt.config].location[clt.location].upload_store.substr(0,place);
					std::cout << temp<<std::endl;
					web.config[clt.config].location[clt.location].upload_store = temp;
					place = web.config[clt.config].location[clt.location].upload_store.size() - 1;
				}
				std::cout << " "<<extension<<std::endl;
				std::string destination = web.config[clt.config].location[clt.location].upload_store + "/" + extension;
			
				out.open(destination,std::ios::out);
				if(!out.is_open())
				{
					std::cerr << "Error: out file\n";
					return  error(clt, 500);
				}
				input.open(temp_str2,std::ios::in);
				if(!input.is_open())
				{
					std::cerr << "Error: input file\n";
					return  error(clt, 500);
				}
				std::string line;
				std::string all = "";
				while(getline(input,line))
				{
					all += line;
					all += "\n";
				}
				out << all;
				out.close();
				input.close();
				if (remove(temp_str2.c_str()) != 0) {
        			std::cerr << "Error deleting the original file: " << clt.upload_files[0].filename << std::endl;
        			return  error(clt, 500);
    			}

				return clt.response.statusCode = 201;
			}
			else if(access(web.config[clt.config].location[clt.location].upload_store.c_str(),F_OK) != 0)
			{
				return error(clt,404);
			}
			else if(access(web.config[clt.config].location[clt.location].upload_store.c_str(),X_OK) != 0 )
			{
				return error(clt,403);
			}
		}	
	}

	std::cout<< clt.map_request["URI"].c_str()<<"Built "<<std::endl;
	if(stat(clt.map_request["URI"].c_str(),&pathStat2) != 0)
		return (error(clt,404));

	if(S_ISDIR(pathStat2.st_mode))
	{
		
		if(*clt.map_request["URI"].rbegin() != '/')
		{
			clt.map_request["URI"] += "/";
			clt.response.body = true;
			clt.response.statusCode = 301;
		}
		if(*clt.map_request["URI"].rbegin() == '/')
		{
			if(clt.location >= 0 && web.config[clt.config].location[clt.location].index.empty())
				return error (clt,403);
			else if(clt.location >= 0 && !web.config[clt.config].location[clt.location].index.empty())
			{
				path2 = clt.map_request["URI"] + web.config[clt.config].location[clt.location].index;
				if(web.config[clt.config].location[clt.location].cgi.empty())
				{
					return (error(clt,403));
				}
				else
				{
					if(!stat(path.c_str(),&pathStat3))
					{			
						
						if(!S_ISDIR(pathStat3.st_mode))
						{
							clt.map_request["URI"] = path2;
							if ( cgi(web,clt))
								return 0;
							return (error(clt,403));
						}
						else
							return (error(clt,403));
					}
					else
						return (error(clt,403));
				}
			}
		}
	}
	if (!S_ISDIR(pathStat2.st_mode))
	{
		std::cout << "File"<<std::endl;
		if(web.config[clt.config].location[clt.location].cgi.empty())
		{
			return (error(clt,403));
		}
		else
		{	
			std::cout << "Wtp"<<std::endl;
			if ( cgi(web,clt))
				return 0;
			return (error(clt,403));
		}
	}
	return clt.response.statusCode = 500;
}