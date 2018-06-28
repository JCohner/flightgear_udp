/* udpserver.c */ 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define PORT "5500"


int main(int argc, char* argv[])
{
    int sock_in, sock_out;
    int addr_len, bytes_read;
    char recv_data[1024];
    struct sockaddr_in server_addr_in , client_addr_in;
    struct sockaddr_in server_addr_out;
    struct addrinfo hints, *servinfo, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int rv, sockfd;
    char msg[50];

    /*error check argument inputs*/
    if (argc != 2) {
    	printf("meant to connect to server IP!\n");
        return 1;
    }

    /*
    	error check get addr info, else put into servinfo
    	currently: 10.1.12.128 and PORT is 5500 
    */
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
       	printf("getaddrinfo failed! rv = %d \n",rv);
       	printf("error def: %s\n", gai_strerror(rv));
       	return -1;
    }
	
	/*error check and initialize obtaining sockfd*/
	if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
    	printf("socket file descriptor init failed\n");
    	printf("error def: %s\n", gai_strerror(sockfd));
        return 1;
    }
    
    /*error check and intialize connection to fg server*/
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
    	close(sockfd);
        printf("connection failed to open\n");
        return 1;
    }
	return 0;
}