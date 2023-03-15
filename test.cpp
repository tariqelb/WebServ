#include <string>
#include <iostream>

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
	return (0);
}
