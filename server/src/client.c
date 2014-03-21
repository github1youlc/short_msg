#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include "../include/socketConnect.h"


int createClientSocket(char * ip, int port) {
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		return -1;
	}
	struct sockaddr_in serverAddress;
	memset(&serverAddress,0, sizeof(serverAddress));
	int success = inet_pton(AF_INET, ip, &serverAddress.sin_addr);
	if (!success) {
		return -1;
	}
	serverAddress.sin_port = htons(port);
	serverAddress.sin_family=AF_INET;

	success = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if (success == -1) {
		return -1;
	}
	return clientSocket;
}



