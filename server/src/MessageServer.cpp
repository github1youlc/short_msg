/*
 * MessageServerThread.cpp
 *
 *  Created on: Mar 21, 2014
 *      Author: yang
 */

#include "../include/MessageServer.h"
#include "../include/socketConnect.h"
#include "../include/cmd.h"
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<pthread.h>
#include <string>
using std::string;

MessageServer::MessageServer(int port, int backlog, int bufferSize) {
	this->port = port;
	this->backlog = backlog;
	this->serverSocket = -1;
	this->bufferSize = bufferSize;
}
bool MessageServer::init() {
	int serverSocket = createServerSocket(messageServerPort, backlog);
	if (serverSocket == -1) {
		return false;
	}
	this->serverSocket = serverSocket;
	return true;
}

void MessageServer::start() {
	while (1) {
		int *clientSocket = malloc(sizeof(int));
		*clientSocket = acceptClientSocket(this->serverSocket);
		if (*clientSocket != -1) {
			pthread_t thread;
			if (0 != (pthread_create(&thread, NULL, this->run, clientSocket))) {
				printf("pthread_create() error\n");
				break;
			}
		}
	}
}

int MessageServer::sendMessage(string fromUser, string toUser, string message) {
	return 0;
}

vector<string> MessageServer::receiveMessages(string user) {
	return NULL;
}
struct cmd_msg * MessageServer::toCommand(string cmd) {
	return NULL;
}

void * MessageServer::run(void * argument) {
	int readCount;
	int clientSockfd = *((int *) argument);
	char buff[this->bufferSize] = { '\0' };
	string cmd;
	int leftBrace = 0;
	int rightBrace = 0;
	char * current = buff;
	int size = this->bufferSize;
	while (1) {
		readCount = recv(clientSockfd, current, size, 0);
		if (readCount < 0) {
			printf("recv fail!!\readCount");
			break;
		}
		bool find = false;
		if (readCount > 0) {
			int i = 0;
			for (; i < readCount; i++) {
				if (buff[i] == '{') {
					leftBrace++;
				} else if (buff[i] == '}') {
					rightBrace++;
				}
				if ((leftBrace == rightBrace) && (leftBrace > 0)) {
					find = true;
					break;
				}
			}
			if (find) {
				cmd.append(current, i);
				cmd_msg * msgcmd = this->toCommand(cmd);
				if (msgcmd != NULL) {
					if (msgcmd->type == 1) {
						cmd_sendMsg * msgsend = (cmd_sendMsg *) msgcmd;
						this->sendMessage(msgsend->fromUserId,
								msgsend->toUserId, msgsend->message);
					} else {
						cmd_getMsg * msgget = (cmd_sendMsg *) msgcmd;
						this->receiveMessages(msgget->userID);
					}

				}
				// 目前一个连接只处理一个命令
				break;
			} else {
				cmd.append(current, readCount);
				current += readCount;
				size -= readCount;
				if (size == 0) {
					current=buff;
					size=this->bufferSize;
				}
			}
		}

	}
}

printf (buff);

free (clientSocket);
close (clientSockfd);
return NULL;
return NULL;
}
MessageServer::~MessageServer() {
// TODO Auto-generated destructor stub
}

