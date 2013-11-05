/*
	Name: Karl James
	Class: Operating Systems
	Due: 11/5/2013
	Talker
	A demonstration of socket programing, the client portion which talks to a server.

	http://www.codeproject.com/Articles/10649/An-Introduction-to-Socket-Programming-in-NET-using
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <arpa/inet.h>
#include <unistd.h>

#define HISPORT 20027

int main(int argc, char *argv[])
{
	int sockfd;
	int i = 0;
	struct sockaddr_in listener_addr;
	struct hostent* he;
	struct in_addr addr;
	int numbytes;
	char str[256];

	/*
		this line checks that the host name and a message were defined when calling the application.
	*/
	if(argc != 3)
	{
		fprintf(stderr, "usage: talker hostname message\n");
		exit(1);
	}

	/*
		This line will resolve the IP address or hostname that was passed so that we can call it later on.
	*/
	if((he = gethostbyname(argv[1])) == NULL)
	{
		perror("gethostbyname");
		exit(1);
	}

	/*
		Here we are retrieving the resolved IP address of our target so we can print it to the screen.
	*/
	addr.s_addr = *(u_long*)he->h_addr_list[i++];
	printf("IPv4 Address #%x: %x\n", i, addr.s_addr);

	/*
		The socket function allocates a file descriptor that represents a socket based on the address familly and protocols provided.
	*/
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("sendto");
		exit(1);
	}

	/*
		The next four lines are configuring the address familly (internet, local, etc.), port, and network card which will be used to bind our socket to a particular port.
	*/
	listener_addr.sin_family = AF_INET;
	listener_addr.sin_port = htons(HISPORT);
	listener_addr.sin_addr = *((struct in_addr*)he->h_addr);
	memset(&(listener_addr.sin_zero), 0,8);

	/*
		The line below is what actually send the message. 
		It accepts the target address and port and sends the message there.
		This is the other half of the recfrom in listen.
	*/
	if((numbytes=sendto(sockfd, argv[2], strlen(argv[2]), 0, (struct sockaddr*)&listener_addr, sizeof(struct sockaddr))) == -1)
	{
		perror("sendto");
		exit(1);
	}

	printf("talker::sent %d bytes to %s\n", numbytes, inet_ntoa(listener_addr.sin_addr));
	
	/*
		This line closes our socket and ends any comunication with the listener.
	*/
	close(sockfd);

	return 0;
}