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
MYSQL * con_relation;



// handle register functions 
user_friends * handle_getFriends(cmd_getFriends * cmd_info)
{
	user_friends * mysql_res;
	// try to send add friend message to the peer
	pthread_mutex_lock(&con_mutex);
	mysql_res = user_get_friends(cmd_info, con_relation);
	pthread_mutex_unlock(&con_mutex);

	print_userFriends(mysql_res);

	return mysql_res;
}



void *recv_data(void *fd)
{
	int client_sockfd;
	client_sockfd=*((int*)fd);

	cmd_getFriends * cmd_info;	
	user_friends *res;
	string cmd_json = get_cmd_from_sockfd(client_sockfd);
	printf("cmd: %s\n", cmd_json.c_str());
	if (cmd_json.size() == 0)
	{
		res = new user_friends;
		res->count = 0;
		thread_response_client(fd, client_sockfd, res);
	}
	cmd_info = get_getFriends_info(cmd_json);
	if (cmd_info == NULL) // the json command is not valid
	{
		res = new user_friends;
		res->count = 0;
		thread_response_client(fd, client_sockfd, res);
	}
	res = handle_getFriends(cmd_info);
	delete cmd_info; // free reg_info
	thread_response_client(fd, client_sockfd, res);
	return NULL;
}

int main()
{
	// initialize all database connections
	con_relation = mysql_connect_db("127.0.0.1", "root", "123456", "relation");
	int server_fd;
	int backlog = 40;

	// create daemon process
	//if(daemon(0,0) == -1)
	//	ERR_EXIT("daemon error");

	pthread_mutex_init(&con_mutex, NULL);
	server_fd = createServerSocket(SERVICE_GET_FRIENDS_PORT, backlog);
	if (server_fd == -1)
	{
		printf("Error: create server socket\n");
		return 0;
	}
	while(1){
		int * client_fd = (int *)malloc(sizeof(int));
		printf(">>> SERVICE GET FRIENDS, warting for connection\n");
		if((*client_fd =acceptClientSocket(server_fd)) == -1)
		{
			perror("socket accept");
			continue;
		}
		printf(">>> SERVICE GET FRIENDS, new connection :%d\n", *client_fd);
		pthread_t thread;
		if(pthread_create(&thread, NULL, recv_data, client_fd)){
			perror("pthread create");
			continue;
		}
	}
	return 0;
}
#endif