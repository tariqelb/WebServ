#include <string>
#include <iostream>

# define MAX_PORT 4

const char  ports[MAX_PORT][6] = {"8080", "8081", "8082", "8083"};

int	main()
{
	std::string	s = "fdkgkdfjbhdf kufhgcvuchudfkcvkghdfbkfdcvbkubh kudfghbb kuhbkuuuuy gdfk";
	std::cout << "size : " << s.size() << std::endl;
	std::cout << "content : " << s << std::endl;
	std::cout << s.max_size() << std::endl;
	/*std::string::iterator it;
	it = s.begin();
	while (it != s.end())
	{
		*it = 0;
		it++;
	}*/
	s.clear();
	std::cout << "size : " << s.size() << std::endl;
	std::cout << "cap : " << s.capacity() << std::endl;
	std::cout << "content : " << s << std::endl;
	char line[100] = "llkdjdfjh";
	s = s + line;
	std::cout << "content : " << s << std::endl;
	std::cout << "size : " << s.size() << std::endl;
	std::cout << "length : " << s.length() << std::endl;
	s.assign("hereis some data  data . 100-continue ");
	std::cout << "content : " << s << std::endl;
	
	
	int rslt = s.find("100-continue");
	std::cout << "find: " << rslt << std::endl;
	rslt = s.find("100-contimue");
	std::cout << "find: " << rslt << std::endl;
	rslt = s.find("001");
	std::cout << "find: " << rslt << std::endl;
	rslt = s.find("hereis");
	std::cout << "find: " << rslt << std::endl;
	int i = 0;
	while (i < MAX_PORT)
	{
		std::cout << "Port : " << ports[i] <<  std::endl;
		i++;
	}
	return (0);
}
