/*
 * messageServer.c
 *
 *  Created on: Mar 21, 2014
 *      Author: yang
 */

#include "../include/socketConnect.h"
#include "../include/cmd.h"
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<pthread.h>
#include <string>
using std::string;

const int messageServerPort = 6001;
const int backlog = 10;
#define MAXLINE 40960




void * clientHandleThread(void *clientSocket) {
	printf("this is a test sentence");
	int clientSockfd;
	int readCount;
	clientSockfd = *((int *) clientSocket);
	char buff[MAXLINE] = { 0 };
	while (1) {
		readCount = recv(clientSockfd, buff, MAXLINE, 0);
		if (readCount <= 0) {
			printf("recv fail!!\readCount");
			break;
		}
		struct cmd_msg * cmd=toCommand(buff);
		if(cmd!=NULL){
			if(cmd->type==1){
				struct cmd_getMsg* getCmd=(struct cmd_getMsg*)cmd;
				receiveMessage(getCmd->userID.c_str(),)
			}
		}

	}



	free(clientSocket);
	close(clientSockfd);
	return NULL;
}

int main(int argc, char ** args) {
	int serverSocket = createServerSocket(messageServerPort, backlog);
	if (serverSocket == -1) {
		err(1, "Can not create server socket!");
	}


}

