/*
 * test.c
 *
 *  Created on: Mar 21, 2014
 *      Author: yang
 */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<pthread.h>
#include <fcntl.h> // 写文件的头
#include "../include/socketConnect.h"
#define MAXLINE 40960

// 处理客户端连接的线程。
void * clientHandleThread(void *clientSocket) {
	printf("this is a test sentence");
	int clientSockfd;
	int readCount;
	clientSockfd = *((int *) clientSocket);
	char buff[MAXLINE] = { 0 };
	while (1) {
		readCount = recv(clientSockfd, buff, MAXLINE, 0);
		if (readCount < 0) {
			printf("recv fail!!\readCount");
			break;
		}
		if(readCount==0){
			break; //finished
		}
	}

	printf(buff);

	free(clientSocket);
	close(clientSockfd);
	return NULL;
}
int main(int argc, char **argv) {
	// 1. 创建server套接字
	int serverSocket = createServerSocket(6667, 1000);
	if (-1 == serverSocket) {
		printf("socket() error\n");
		exit(1);
	}
	printf("==============server waiting for connect====================\n");
	int * clientSocket = NULL;
	while (1) {
		int * clientSocket = malloc(sizeof(int));
		*clientSocket = acceptClientSocket(serverSocket);
		if (*clientSocket == -1) {
			err(1, "accept client socket failed.");
		}

		// single thread version
		clientHandleThread(clientSocket);
		// multi thread version
		pthread_t thread;
//		if (0
//				!= (pthread_create(&thread, NULL, clientHandleThread,
//						clientSocket))) {
//			printf("pthread_create() error\n");
//			break;
//		}
	}
	// 出错跳出，要关闭最后一个套接字，并释放其空间。
	close(clientSocket);
	free(clientSocket);
	close(serverSocket);
	exit(1);
}
