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

#define PORT_IN "1917"
#define PORT_OUT "1918"
#define FLIGHTGEAR_IP "10.1.12.128"
#define VM_IP "127.0.0.1" //VM in office is @ 10.1.12.61

/*
    these parameters setup the command line calls on the computer running
    flight gear from C:\Program Files\FlightGear 2018.2.2\bin
    >fgfs --generic=socket,in,60,,$(PORT_IN),udp,input_protocol
        --generic=socket,out,60,$(VM_IP),$(PORT_OUT),udp,output_protocol
        --aircraft=$(CRAFT_NAME) --fdm=null
*/

struct connector_packet {
    int sockfd;
    struct sockaddr_in ai_addr;
}

struct connector_packet udp_setup(const char * ip, const char * port) {
    struct connector_packet cp;
    struct addrinfo hints, *servinfo, *p;
    int rv, rv1, sockfd;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if (ip == VM_IP){
        hints.ai_flags = AI_PASSIVE;
            /* error check get addr info, else put into servinfo */
            if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
            printf("getaddrinfo failed! rv = %d \n",rv);
            printf("error def: %s\n", gai_strerror(rv));
            return -1;
        } else {
            printf("addr info obtained for sock_out!\n");
        }
    } else {
        /* error check get addr info, else put into servinfo */
        if ((rv = getaddrinfo(ip, port, &hints, &servinfo)) != 0) {
            printf("getaddrinfo failed! rv = %d \n",rv);
            printf("error def: %s\n", gai_strerror(rv));
            return -1;
        } else {
            printf("addr info obtained!\n");
        }
    }  
    /* error check and initialize obtaining sockfd */
    if ((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
        printf("socket file descriptor init failed\n");
        printf("error def: %s\n", gai_strerror(sockfd));
        return 1;
    } else {
        printf("socket file descriptor obtained!\n");
    }

    if (ip == FLIGHTGEAR_IP) {
        /* error check and intialize connection to fg server */
        if ((rv = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) < 0) {
            close(sockfd);
            printf("connection failed to open\n");
            printf("error number is: %d def: %s\n", errno, gai_strerror(errno));
            return 1;
        } else {
            printf("no error in datagram in socket connect!\n");
        }
    } else {
        if((rv = bind(sockfd,servinfo->ai_addr,servinfo->ai_addrlen)) < 0) {
            close(sockfd);
            printf("bind failed\n");
            printf("error number is: %d def: %s\n", errno, gai_strerror(errno));
            return 1;
        } else {
            printf("no error in datagram out socket bind!\n");
        }
    }
    cp.sockfd = sockfd;
    cp.ai_addr = servinfo->ai_addr;

    freeaddrinfo(servinfo);
    return cp;
}

struct data_in_packet{
    /*position*/
    float altitude; //(ft)
    float latitude; //(deg)
    float longitude; //(deg)
    
    /*attitude*/
    float pitch; //(deg)
    float roll; //(deg)
    float yaw; //(heading)(deg)

    /*fin angle*/
    /*
    float fin_TL; //(deg)
    float fin_TR; //(deg)
    float fin_BL; //(deg)
    float fin_BR; //(deg)
    */
    /*motor angle*/
    /*
    float m1;
    float m2;
    float m3;
    float m4;
    float m5;
    float m6;
    float m7;
    float m8;
    float m9;
    float m10;
    float m11;
    float m12;
    */
};

struct data_out_packet{
    double altitude;
};

int main(int argc, char* argv[])
{
    /*declare socket data structures for passing*/
    struct data_in_packet dp_in;
    struct data_out_packet dp_out;

    /*declare structures for persisting relevant data*/
    struct connector_pakcet cp_in, cp_out;
    struct sockaddr_in addr_out; 
    int sockfd_in, sockfd_out;
    int msg_len_in, msg_len_out, addr_len;

    /*call socket setups, persist relevant data*/
    cp_in = udp_setup(FLIGHTGEAR_IP, PORT_IN);
    sockfd_in = cp_in.sockfd;
    cp_out = udp_setup(VM_IP, PORT_OUT);
    sockfd_out = cp_out.sockfd;
    addr_out = cp_out.ai_addr;

    /*declare and intialize time structures*/
    struct timespec time;
    time.tv_nsec = 16666666; //will make a 60Hz timer 
    time.tv_sec = 0;;
    
    /*define initial input values*/
    dp_in.altitude = 1000;
    dp_in.longitude = -157.9270976;
    dp_in.latitude = 21.31394427;
    dp_in.pitch = 0;
    dp_in.roll = 0;
    dp_in.yaw = 90;

    /*obtain length of structures passed through sockets*/
    msg_len_in = sizeof(dp_in);
    msg_len_out = sizeof(dp_out);
    addr_len = sizeof(struct sockaddr);
    while(1){
        nanosleep(&time,NULL);
        dp_in.longitude += 0.00001;
        dp_in.roll += 0.05;

        // Send UDP packet
        printf("Sending %d bytes of data \n", msg_len_in);
        send(sockfd_in, &dp_in, msg_len_in, 0);

        // Recieve UDP packet
        recvfrom(sockfd_out, &dp_out, msg_len_out, 0, (struct sockaddr *)&addr_out,&addr_len);
        printf("Reading altitude value of: %d\n", dp_out.altitude);

    }
	return 0;
}