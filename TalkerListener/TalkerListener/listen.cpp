/*
	Name: Karl James
	Class: Operating Systems
	Due: 11/5/2013
	Listen
	A demonstration of socket programing, the server portion which listens for requests.
	
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

#define MYPORT 20027
#define MAXBUFLEN 100

int main()
{
	int sockfd, addr_len, numbytes;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	char buf[MAXBUFLEN];
	char str[MAXBUFLEN];

	/*
		The socket function allocates a file descriptor that represents a socket based on the address familly and protocols provided.
	*/
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	/*
		The next four lines are configuring the address familly (internet, local, etc.), port, and network card which will be used to bind our socket to a particular port.
	*/
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(my_addr.sin_zero), 0,8);

	/*
		The bind function is taking our socket created earlier and binding it to a particular port so that port is reserved for our use.
	*/
	if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	addr_len = sizeof(struct sockaddr);
	/*
		The recvfrom function listens on the port and address bound earlier and recieves any bytes passed to it by talker.
	*/
	if((numbytes = recvfrom(sockfd, str, MAXBUFLEN, 0, (struct sockaddr*)&their_addr, (socklen_t*)&addr_len)) == -1)
	{
		perror("recfrom");
		exit(1);
	}
	str[numbytes] = '\0';

	inet_ntop(AF_INET, &their_addr.sin_addr, buf, 256);
	printf("listen::got packet from %s\n", buf);

	printf("listen::packet is %d bytes long\n", numbytes);
	printf("listen::packet contains \"%s\"\n", str);

	/*
		This line closes the file descriptor of our socket, which frees up the port we were listening on.
	*/
	close(sockfd);
}