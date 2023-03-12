#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 1024

int	main()
{
	char	buffer[1024];
	int		sock;
	struct sockaddr_in hostadd;
	char response[] = "HTTP/1.0 200 OK\r\n"
                  "Server: webserver-c\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html>hello, world</html>\r\n";
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		printf("%s\n", "Error in socket creating");
	else
	{
		printf("%s\n", "Created succefully");	
	}
	hostadd.sin_family = PF_INET;
	hostadd.sin_port = htons(8080);
	hostadd.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sock, (struct sockaddr *)&hostadd, sizeof(hostadd)) != 0)
		printf("%s\n", "Bind error occur");
	else
		printf("%s\n", "binded");
	struct sockaddr_in	client_addr;
	int					client_addlen = sizeof(client_addr);		
 	printf("sin fam : %d\n", hostadd.sin_family);
	printf("sin por : %d\n", hostadd.sin_port);
	printf("sin add : %d\n", hostadd.sin_addr.s_addr);
	printf("sin add : %d\n", INADDR_ANY);
	
	if (listen(sock, SOMAXCONN) != 0)
		printf("%s\n", "listen error");
	else
		printf("%s\n", "En listen mode");

	while (1)
	{
			int len = sizeof(hostadd);
			int newsock = accept(sock, (struct sockaddr *) &hostadd , (socklen_t *) &len);
			if (newsock < 0)
				printf("%s\n", "Accept error");
			printf("%s\n", "Accepted");
			

			int len2 = sizeof(client_addr);
			int sockname  = getsockname(newsock, (struct sockaddr *) &client_addr, (socklen_t *) &len2);
			if (sockname < 0)
				printf("%s\n", "Sockname error");



			int red = read(newsock, buffer, 1024);
			if (red < 0)
				printf("%s\n", "read error");
			printf("%s\n%s\n", "Reading", buffer);
			char method[BUFFER_SIZE], uri[BUFFER_SIZE], version[BUFFER_SIZE];
        	sscanf(buffer, "%s %s %s", method, uri, version);
			
		
		
			printf("{%s}:[%u]\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
			printf("{%s}\n, {%s}\n, {%s}\n", method, uri, version);


			int wrt = write(newsock, response, strlen(response));
			if (wrt < 0)
				printf("%s\n", "write error");
			printf("%s\n", "write");

			close(newsock);

	}

	return (0);
}

