#define _WINSOCKAPI_
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <io.h>


#pragma comment(lib,"ws2_32.lib") //winsock lib
#define port_number 5013

void *get_in_addr(struct sockaddr *sa){
	if(sa->sa_family == AF_INET){
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	//return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main (int argc, char *argv[]){
	WSADATA wsa;
	SOCKET s, new_socket;
	struct sockaddr_in server, client;
	int c;
	//char * message;
	char message[32];

	printf("\nInitializing Winsock");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0){
		printf("initialization failed. error code %d",WSAGetLastError());
		return 1;
	}

	printf("Initialized.\n");

	//Create socket
	if((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET){
		printf("Could not create socket: %d",WSAGetLastError());
		return 1;
	}

	printf("socket created\n");

	//prepare sockaddr_in struct
	memset((void *)&server, '\0', sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY; //this machine is host
	server.sin_port = htons(port_number);

	//bind -tcp server
	if(bind(s,(struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR){
		printf("Bind failed with error code : %d", WSAGetLastError());
		return -1;
	}

	puts("Bind done");
	//listen to incoming connections
	listen(s, 3);

	printf("server: waiting for connections...\n");

	while(1) {
		c = sizeof(struct sockaddr_in);
		new_socket = accept(s, (struct sockaddr *)&client, &c);
		if(new_socket == INVALID_SOCKET){
			perror("accept");
			continue;
		}

		//inet_ntop(client.sin_family, get_in_addr((struct sockaddr *)&client),message,sizeof(message));
		printf("server: got connection!");

		close(new_socket);
	}


	

	
	/*
	puts("connection accepted\n");
	

	//reply to client
	message = "hello client, I have received your connection, but I have to go now, bye fam \n";
	send(new_socket, message, strlen(message), 0);

	getchar();
	*/
	closesocket(s);
	WSACleanup();

	return 0;

}