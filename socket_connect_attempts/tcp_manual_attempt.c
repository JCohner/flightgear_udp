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

#define PORT "5013"


int main(int argc, char* argv[])
{
    int sock_in, sock_out;
    int addr_len, bytes_read;
    char recv_data[1024];
    struct sockaddr_in server_addr_in , client_addr_in;
    struct sockaddr_in server_addr_out;
    int msgL = 0;
    struct sockaddr_in server_addr_out;
    struct hostent *host_out;
    char send_data[1024];
    /* using older ipv4 population method 
    struct addrinfo hints, *servinfo, *p;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int rv, sockfd;
    char msg[50];
    */
    /*error check argument inputs*/
    if (argc != 2) {
    	printf("meant to connect to server IP!\n");
        return 1;
    }

    //obtain socket file descriptor
    if ((sock_in = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("datagram socket connection error - server side");
        return 1;
    }
    server_addr_in.sin_family = AF_INET;
    server_addr_in.sin_port = htons(5013);
    server_addr_in.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr_in.sin_zero),8); //zero padding bytes
    if (bind(sock_in,(struct sockaddr *)&server_addr_in,sizeof(struct sockaddr)) == -1) {
        printf("Bind error\n");
        return -1;
    }
    addr_len = sizeof(struct sockaddr);
        
    printf("\nUDPServer Waiting for client on port 5013");
    fflush(stdout);

    // client config
    host_out = (struct hostent *) gethostbyname((char *)"127.0.0.1");

    if ((sock_out = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("datagram socket connection error - client side");
       return 1;
    }

    server_addr_out.sin_family = AF_INET;
    server_addr_out.sin_port = htons(5018);
    server_addr_out.sin_addr = *((struct in_addr *)host_out->h_addr);
    bzero(&(server_addr_out.sin_zero),8);
    /*
    // Construct a packet to send over UDP with control inputs    
    msgL = sprintf(send_data, "%f\t%f\n", aileron_ctrl, pitch_ctrl);
    // Print Control inputs to stdout
    printf("\nControl Signal\n%s\n", send_data);          
    // Send UDP packet
    sendto(sock_out, send_data, strlen(send_data), 0, (struct sockaddr *)&server_addr_out, sizeof(struct sockaddr));
	return 0;
    */
    return 0;
}