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

#define PORT "1917"


int main(int argc, char* argv[])
{
    int addr_len, bytes_read;
    char recv_data[1024];
    char send_data[1024];
    struct addrinfo hints, *servinfo, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    int rv, sockfd;
    int msg_len = 0;
    float altitude = atof(argv[2]);
    float pitch = atof(argv[3]);
    float roll = atof(argv[4]);
    float yaw = atof(argv[5]);

    /*error check argument inputs*/
    if (argc != 6) {
    	printf("error:\nintended syntax:(sever_ip,altitude-ft,pitch,roll,yaw)\n");
        return 1;
    }

    /*
    	error check get addr info, else put into servinfo
    	currently: 10.1.12.128 and PORT is 5001
    */
    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
       	printf("getaddrinfo failed! rv = %d \n",rv);
       	printf("error def: %s\n", gai_strerror(rv));
       	return -1;
    } else {
        printf("addr info obtained!\n");
    }
	
	/*error check and initialize obtaining sockfd*/
	if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
    	printf("socket file descriptor init failed\n");
    	printf("error def: %s\n", gai_strerror(sockfd));
        return 1;
    } else {
        printf("socket file descriptor obtained!\n");
    }
    
    /*error check and intialize connection to fg server*/
    if ((rv = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) < 0) {
    	close(sockfd);
        printf("connection failed to open\n");
        printf("error number is: %d def: %s\n", errno, gai_strerror(errno));
        return 1;
    } else {
        printf("no error in datagram socket connect!\n");
    }

    msg_len = sprintf(send_data, "%.2f\t%.2f\t%.2f\t%.2f\n", altitude,pitch,roll,yaw);
    // Print Control inputs to stdout
    printf("\nalt\tpitch\troll\tyaw:\n%s\n", send_data);          
    // Send UDP packet
    printf("attempting to send %d bytes of data \n", msg_len);
    send(sockfd, send_data, msg_len, 0);

	return 0;
}