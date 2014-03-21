#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../include/socketConnect.h"
// 创建一个服务器套接字
int createServerSocket(int port, int backlog) {
	// 创建socket
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == -1) {
		return -1;
	}
	// 为socket指定地址
	struct sockaddr_in serverAddress;
	memset(&serverAddress,0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	int success = bind(serverSocket, (struct sockaddr *) &serverAddress,
			sizeof(serverAddress));
	if (success == -1) {
		return -1;
	}
	// 让socket开始接收信息
	success = listen(serverSocket, backlog);
	if (success == -1) {
		return -1;
	}
	return serverSocket;
}

// 接收一个客户端套接字
int acceptClientSocket(int serverSocket) {
	int clientSocket = accept(serverSocket, NULL, NULL);
	return clientSocket;
}

