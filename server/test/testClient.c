/*
 * testClient.c
 *
 *  Created on: Mar 21, 2014
 *      Author: yang
 */
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include "../include/socketConnect.h"
#define MAXLINE 4096
int main(int argc, char** argv) {
	int sockfd;
	struct sockaddr_in server_address;
	char * data = "{this is a test}";
	int i = 0;
	printf("connect number = %d\n", i + 1);
	sockfd = createClientSocket("127.0.0.1", 6667);
	if (sockfd == -1) {
		err(1, "create client socket failed");
	}
	if (send(sockfd, data, strlen(data), 0) <= 0) {
		printf("send() error\n");
		close(sockfd);
		return 1;

		close(sockfd);
	}
	return 0;
}
