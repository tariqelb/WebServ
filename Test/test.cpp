#include <iostream>
#include <map>

int	main()
{
	std::map<std::string, std::string> request;
	std::pair<std::string, std::string> pr;
	std::map<std::string, std::string>::iterator it;
	
	pr.first = "Host";
	pr.second = "Second a";
	it = request.end();
	request.insert(it, pr);


	pr.first = "First b";
	pr.second = "Second b";
	it = request.end();
	request.insert(it, pr);


	pr.first = "First c";
	pr.second = "Second c";
	it = request.end();
	request.insert(it, pr);


	it = request.begin();
	while (it != request.end())
	{
		std::cout << it->first << " " << it->second << std::endl;
		it++;
	}

	return (0);
}
