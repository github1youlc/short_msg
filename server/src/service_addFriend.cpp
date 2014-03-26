#ifndef SHORT_MSG_SERVICE_REGISTER_CPP
#define SHORT_MSG_SERVICE_REGISTER_CPP

#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>
using std::string;

#include "cmd.h"
#include "mysql_helper.h"
#include "common.h"
#include "json_helper.h"
#include "exten_def.h"

extern "C" {
#include "socketConnect.h"
}




//define error exit
#define ERR_EXIT(m) \
	do\
{\
	perror(m);\
	exit(EXIT_FAILURE);\
}\
	while (0);\





pthread_mutex_t con_mutex;
MYSQL * con_msg;
MYSQL * con_user;


// handle register functions 
request_res * handle_addFriend(cmd_addFriend * cmd_info)
{
	request_res * res = new request_res;	// eventual result
	int tmp_res;         // store result every step

	// try to send add friend message to the peer
	pthread_mutex_lock(&con_mutex);
	tmp_res = user_add_friend(cmd_info, con_msg, con_user);
	pthread_mutex_unlock(&con_mutex);

	if (tmp_res == -1)
	{
		res->result = -1;
		res->msg = "mysql operation failed";
		return res;
	}else if (tmp_res == 0)
	{
		res->result = 0;
		res->msg = "user to add not exist";
		return res;
	}else if (tmp_res == 1)
	{
		res->result = 1;
		res->msg = "add request is sended succeed";
		return res;
	}else{
		delete res;
		return NULL;
	}
	return res;
}



void *recv_data(void *fd)
{
	int client_sockfd;
	client_sockfd=*((int*)fd);

	cmd_addFriend * cmd_info;	
	request_res *res;
	string cmd_json = get_cmd_from_sockfd(client_sockfd);
	printf("cmd: %s\n", cmd_json.c_str());
	if (cmd_json.size() == 0)
	{
		res = new request_res;
		res->result = -2;
		res->msg = "socket recv data error";
		thread_response_client(fd, client_sockfd, res);
	}
	cmd_info = get_addFriend_info(cmd_json);
	if (cmd_info == NULL) // the json command is not valid
	{
		res = new request_res;
		res->result = -2;
		res->msg = cmd_json + ", not invalid json commond";
		thread_response_client(fd, client_sockfd, res);
	}
	res = handle_addFriend(cmd_info);
	delete cmd_info; // free reg_info
	thread_response_client(fd, client_sockfd, res);
	return NULL;
}

int main()
{
	// initialize all database connections
	con_msg = mysql_connect_db("127.0.0.1", "root", "123456", "msg");
	con_user = mysql_connect_db("127.0.0.1", "root", "123456", "user");
	int server_fd;
	int backlog = 40;

	// create daemon process
	//if(daemon(0,0) == -1)
	//	ERR_EXIT("daemon error");

	//initialize the log file mutex
	pthread_mutex_init(&con_mutex, NULL);
	server_fd = createServerSocket(SERVICE_ADD_FRIEND_PORT, backlog);
	if (server_fd == -1)
	{
		printf("Error: create server socket\n");
		return 0;
	}
	while(1){
		int * client_fd = (int *)malloc(sizeof(int));
		printf(">>> SERVICE ADD FRIEND, warting for connection\n");
		if((*client_fd =acceptClientSocket(server_fd)) == -1)
		{
			perror("socket accept");
			continue;
		}
		printf(">>> SERVICE ADD FRIEND, new connection :%d\n", *client_fd);
		pthread_t thread;
		if(pthread_create(&thread, NULL, recv_data, client_fd)){
			perror("pthread create");
			continue;
		}
	}
	return 0;
}
#endif