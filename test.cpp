#include <iostream>
#include <string>


int	main(void)
{
	std::string str;
	str.assign("   server data { data   	");
	std::cout << "find :  " <<  str.find ("server ") << "  " << str << std::endl;
	str.assign("allow GET; allow POST; allow DETETE;");
	std::cout << "find :  " <<  str.find (";") << "  " << str << std::endl;
	std::cout << "find :  " <<  str.find (";") << "  " << str << std::endl;
	std::cout << "find :  " <<  str.find (";") << "  " << str << std::endl;
	std::cout << "find :  " <<  str.find (";") << "  " << str << std::endl;
	
	std::cout << str.size() << std::endl;
	std::string data = "data;data;";
	data[4] = ' ';
	std::cout << "STR : " << data << std::endl;
	return (0);
}
