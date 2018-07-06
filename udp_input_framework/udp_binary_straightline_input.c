/* udpserver.c */ 

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

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

struct datapacket{
    float altitude;
    //char var_sep_0;
    float latitude;
    //char var_sep_1;
    float longitude;
    //char var_sep_2;
    float pitch;
    //char var_sep_3;
    float roll;
    //char var_sep_4;
    float yaw;
    //char newline;
};


int main(int argc, char* argv[])
{
    struct datapacket dp;
    dp.altitude = 1000;
    float longitude_o = -157.9270976;
    float latitude_o = 21.31394427;
    dp.pitch = 0;
    dp.roll = 0;
    dp.yaw = 90;
    char var_sep = '\t';
    char newline = '\n';
    /*
    dp.var_sep_1 = '\t';
    dp.var_sep_2 = '\t';
    dp.var_sep_3 = '\t';
    dp.var_sep_4 = '\t';
    */
    udp_connect_setup();
    //char buffer[100];

    dp.latitude = latitude_o;
    dp.longitude = longitude_o;
    

    struct timespec time;
    time.tv_nsec = 16666666; //will make a 60Hz timer 
    time.tv_sec = 0;
    /*put them on circle*/
    dp.longitude += 0;
    dp.latitude += 0;
    int msg_len_data = sizeof(dp.altitude);
    int msg_len_char = sizeof(var_sep);
    while(1){
        nanosleep(&time,NULL);
        dp.longitude += 0.00001;
        dp.roll += 0.05;
        // Send UDP packet
        msg_len = sizeof(dp);
        printf("longitude is: %f\n",dp.longitude);
        printf("attempting to send %d bytes of data \n", msg_len);
        send(sockfd, &dp, msg_len, 0);
        /*
        send(sockfd, &dp.altitude, msg_len_data, 0);
        send(sockfd, &var_sep, msg_len_char, 0);
        send(sockfd, &dp.latitude, msg_len_data, 0);
        send(sockfd, &var_sep, msg_len_char, 0);
        send(sockfd, &dp.longitude, msg_len_data, 0);
        send(sockfd, &var_sep, msg_len_char, 0);
        send(sockfd, &dp.pitch, msg_len_data, 0);
        send(sockfd, &var_sep, msg_len_char, 0);
        send(sockfd, &dp.roll, msg_len_data, 0);
        send(sockfd, &var_sep, msg_len_char, 0);
        send(sockfd, &dp.yaw, msg_len_data, 0);
        send(sockfd, &newline, msg_len_char, 0);
        */
    }
	return 0;
}