#include "webserv.hpp"

int	main()
{
	std::fstream 	s;
	char			line[100];
	std::string		reads;

	s.open("name.txt");
	while (1)
	{
		s.read(line, 99);
		reads.assign(line);
		if (reads.size() == 0)
			break;
		int i = 0;
		while (i < reads.size())
		{
			if (reads[i] == '\r')
				std::cout << "\\r";
			else 
			{
				if (reads[i] == '\n')
					std::cout << "\\n";
				std::cout << reads[i];
			}
			i++;
		}
		reads.assign("");
		memset(line, 0 , 100);
	}
}
