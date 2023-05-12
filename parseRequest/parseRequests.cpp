#include "../webserv.hpp"

void	parseRequests(struct webserv web, std::stringstream& buffer)
{
	std::ofstream ofile;


	ofile.open("name.txt", std::ios::trunc | std::ios::out);
	ofile << buffer.str();
}
