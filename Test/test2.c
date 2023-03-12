#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#define CONNECTION_PORT 3500

int	main(void)
{
	int					status;
	int					serv_socket_descriptor;
	int					client_socket_descriptor;
	char				buffer[80];
	int					length_of_addr;
	struct sockaddr_in	server_address;
	struct sockaddr_in	client_address;
	char				*msg;
	
	msg =  "HTTP/1.0 200 OK\r\n"
		"Server: webserver-c\r\n"
		"Content-type: text/html\r\n\r\n"
		"<html>hello, world</html>\r\n";;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(CONNECTION_PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	printf("start creating\n");
	// create the server socket
	serv_socket_descriptor = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_socket_descriptor < 0)
		return (1);
	// bind the server socket with the value address and port from the sockaddr_in struct
	status = bind(serv_socket_descriptor, (struct sockaddr*) &server_address, sizeof(struct sockaddr));
	if (status < 0)
		return (1);
	// listen to specified port with the maximum of 4 requests
	status = listen(serv_socket_descriptor, 4);
	if (status < 0)
		printf("%s\n", "listen error");
	// accept connection signal from the client
	printf("start listening\n");
	int length_of_client = sizeof(client_address);
	client_socket_descriptor = accept(serv_socket_descriptor, (struct sockaddr *) &client_address, (socklen_t *) &length_of_client);
	if (client_socket_descriptor < 0)
		printf("%s\n", "accept error");
	// receive data sent by the client
	char storage_buffer[200];
	int rd = read(client_socket_descriptor, storage_buffer, 200);
	storage_buffer[rd] = 0;
	printf("%s%s\n", "The buffer message :", storage_buffer);
	// send data to the client 
	send(client_socket_descriptor, msg, strlen(msg), 0);
	while (1);
	close(client_socket_descriptor);
	close(serv_socket_descriptor);
	return (0);
}
