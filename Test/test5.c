#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define MYPORT "3490"  // the port users will be connecting to
#define BACKLOG 10     // how many pending connections queue will hold

int main(void)
{
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *p, *res;
    int sockfd, new_fd;

    // !! don't forget your error checking for these calls !!

    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // use IPv4 or IPv6, whichever
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
    //hints.ai_flags = AI_CANONNAME;     // fill in my IP for me

    int st = getaddrinfo(NULL, MYPORT, &hints, &res);
	if (st != 0)
		return (printf("Error\n"));
	{
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
	printf("net familly : %d : %d : %d\n", AF_INET, AF_UNSPEC , ipv4->sin_family);
	printf("port number : %hu\n", ntohs(ipv4->sin_port));
	void *addr = &(ipv4->sin_addr);
	char ipstr0[16];
	char *ipstr1;
	inet_ntop(res->ai_family, (const void *)&ipv4->sin_addr, ipstr0, sizeof(ipstr0));
	ipstr1 = inet_ntoa(ipv4->sin_addr);

	printf("ip address  : %s\n", ipstr0 );
	printf("ip address  : %s\n", ipstr1 );
	printf("Canon name %s\n", res->ai_canonname);
	printf("--------------------------------------------\n");
	}

    // make a socket, bind it, and listen on it:

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    listen(sockfd, BACKLOG);

    // now accept an incoming connection:

    addr_size = sizeof their_addr;
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
	{
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)&their_addr;
	printf("net familly : %d : %d : %d\n", AF_INET, AF_UNSPEC , ipv4->sin_family);
	printf("port number : %hu\n", ntohs(ipv4->sin_port));
	void *addr = &(ipv4->sin_addr);
	char ipstr0[16];
	char *ipstr1;
	inet_ntop(res->ai_family, addr, ipstr0, sizeof(ipstr0));
	ipstr1 = inet_ntoa(ipv4->sin_addr);

	printf("ip address  : %s\n", ipstr0 );
	printf("ip address  : %s\n", ipstr1 );
	printf("--------------------------------------------\n");
	}
	{
		struct sockaddr_in	new_add;
		char			hostname[32];
		
		socklen_t new_add_size = sizeof(new_add);
		gethostname(hostname, 32);
		printf("host name : %s\n", hostname);
		getpeername(new_fd, (struct sockaddr *) &new_add, &new_add_size);
		printf("*-------------------------------------------\n");
		printf("Remote peer address: %s:%d\n", inet_ntoa(new_add.sin_addr), ntohs(new_add.sin_port));
		printf("*-------------------------------------------\n");
		
		char			hostaddr[32];
		struct in_addr	addr;
		struct hostent	*host;
		host = gethostbyaddr(&(((struct sockaddr_in *)&their_addr)->sin_addr) , sizeof(addr), AF_INET);
		printf("h name       : %s\n", host->h_name);
		printf("h aliasws  : %s\n", host->h_aliases[0]);
		printf("h addr list  : %s\n", host->h_addr_list[0]);
	}
	freeaddrinfo(res);
	return (0);
}
