#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> //defines hostent 	
#include <string.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
	
	//program takes two arguments, hostname(server) and destinated port number.
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);


    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("I'm client with router number 0, link cost:\nrouter1: 1 \trouter2: 3 \trouter3: 7\nthis message will be sent to the server: \n\n");
	strcpy(buffer, "I'm client with router number 0, link cost:\nrouter1: 1 \trouter2: 3 \trouter3: 7\n");
    //bzero(buffer,256);
    //fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)	error("ERROR writing to socket");
    bzero(buffer,256);

	//read message from server
    n = read(sockfd,buffer,255);
    if (n < 0)	error("ERROR reading from socket");
	printf("message from server\n");
    printf("%s\n",buffer);
    return 0;
}
