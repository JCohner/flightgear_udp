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
#define FLIGHTGEAR_IP "10.1.12.128"

int addr_len, bytes_read;
char recv_data[1024];
char send_data[1024];
struct addrinfo hints, *servinfo, *p;
int rv, sockfd;
int msg_len = 0;

int udp_connect_setup(void) {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    /*
        error check get addr info, else put into servinfo
        currently: 10.1.12.128 and PORT is 1917
    */
    if ((rv = getaddrinfo(FLIGHTGEAR_IP, PORT, &hints, &servinfo)) != 0) {
        printf("getaddrinfo failed! rv = %d \n",rv);
        printf("error def: %s\n", gai_strerror(rv));
        return -1;
    } else {
        printf("addr info obtained!\n");
    }

    /*
        error check and initialize obtaining sockfd
    */
    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
        printf("socket file descriptor init failed\n");
        printf("error def: %s\n", gai_strerror(sockfd));
        return 1;
    } else {
        printf("socket file descriptor obtained!\n");
    }

    /*
        error check and intialize connection to fg server
    */
    if ((rv = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) < 0) {
        close(sockfd);
        printf("connection failed to open\n");
        printf("error number is: %d def: %s\n", errno, gai_strerror(errno));
        return 1;
    } else {
        printf("no error in datagram socket connect!\n");
    }
}


int main(int argc, char* argv[])
{
    float altitude;
    float longitude;
    float latitude;
    float pitch;
    float roll;
    float yaw;
    udp_connect_setup();
    //char buffer[100];

    while(1){
        /*inputs of form (altitude,latitude,longitude,pitch,roll,yaw)*/
        printf("input: altitude,latitude,longitude,pitch,roll,yaw\n");
        scanf("%f %f %f %f %f %f",&altitude,&latitude,&longitude,&pitch,&roll,&yaw);

        msg_len = sprintf(send_data, "%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", altitude,latitude,longitude,pitch,roll,yaw);
        // Print Control inputs to stdout
        printf("\nalt\tlat\tlong\tpitch\troll\tyaw:\n%s\n", send_data);          
        // Send UDP packet
        printf("attempting to send %d bytes of data \n", msg_len);
        send(sockfd, send_data, msg_len, 0);
    }
	return 0;
}