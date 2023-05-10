#include <string.h>
#include <stdio.h>

int main(int ac , char **av)
{

	char *str = "POST / HTTP/1.1\nHost: 127.0.0.1:8080\nUser-Agent: curl/7.54.0\nAccept: +++\nContent-Type: application/text\nContent-Length: 3893\nExpect: 100-continue\n\r\n" ;
	printf("%lu\n", strlen(str));
	if (ac > 1)
		printf("%lu\n", strlen(av[1]));

	return (0);
}
