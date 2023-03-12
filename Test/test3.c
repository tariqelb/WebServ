#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>

int	main()
{
	int status;
	struct  addrinfo	hints;
	struct	addrinfo	*servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	//hints.aiflags = AI_PASSIVE;


	status = getaddrinfo("www.google.com", "80", &hints, &servinfo);
	printf("-----------------------\n");
	printf("ai_flags %d \n", hints.ai_flags);
	printf("ai_family %d \n", hints.ai_family);
	printf("ai_socktype %d \n", hints.ai_socktype);
	printf("ai_protocol %d \n", hints.ai_protocol);
	printf("ai_addrlen %d \n",  hints.ai_addrlen);
	printf("ai_canonname %s \n", hints.ai_canonname);
	//printf("sockaddr(sockaddr_in) %d \n", hints.ai_addr->sa_family);
	//printf("sockaddr(sockaddr_in) %d \n", hints.ai_addr->sin_port);
	//printf("sockaddr(sockaddr_in) %d \n", hints.ai_addr->sin_zero);
	//printf("sockaddr(sockaddr_in) %d \n", hints.ai_addr->sin_addr.s_addr);



	return (0);
}
