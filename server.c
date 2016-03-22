/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


/* from netinet/in.h, struct contains the internet address	
struct sockaddr_in {
        short   sin_family;
        u_short sin_port;
        struct  in_addr sin_addr;
        char    sin_zero[8];
};*/

void error(char *msg)
{
    perror(msg);
    exit(1);
}
	
int main(int argc, char *argv[])
{

	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	char serverMsg[256];
	strcpy(serverMsg, "I'm server with router number 1, link cost:\nrouter1: 1 \trouter2: 1\n");
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	//second argument provides the port number
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	//this system call creates a new socket that is of type SOCK_STREAM(TCP)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0) error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));

	//port number
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");
	//listen on created socket for connection
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	//accept returns a new file descriptor once a client has connected to the server
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)	error("ERROR on accept");
	//initialize buffer to 0
	bzero(buffer,256);
	//read from buffer, if successful, the server reads the data sent from client
	n = read(newsockfd,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	printf("message from client: %s\n",buffer);
	n = write(newsockfd, serverMsg, strlen(serverMsg));
	if (n < 0) error("ERROR writing to socket");
	return 0; 
}
