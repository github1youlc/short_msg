/*
 * MessageServerThread.h
 *
 *  Created on: Mar 21, 2014
 *      Author: yang
 */

#ifndef MESSAGESERVERTHREAD_H_
#define MESSAGESERVERTHREAD_H_
#include<string>
#include<vector>
using namespace std;
class MessageServer {
	public:
		MessageServer(int port,int backlog,int bufferSize);
		bool init();
		void start();
		/**
		 * convert the message to command
		 */
		struct cmd_msg * toCommand(string cmd);
		/**
		 * send message from user to user.
		 */
		int sendMessage(string fromUser,string toUser, string  message);

		/**
		 * receive messages.
		 */
		vector<string> receiveMessages(string user);
		/**
		 * the callback method of the new thread.
		 */
		void * run(void *);
		virtual ~MessageServer();
	private:
		/**
		 * Listen port.
		 */
		int port;
		/**
		 * the max number of the accept waiting list.
		 */
		int backlog;
		/**
		 * the server socket used to listen request.
		 */
		int serverSocket;
		/**
		 * the max length of the message to read.
		 */
		int bufferSize;
};

#endif /* MESSAGESERVERTHREAD_H_ */
