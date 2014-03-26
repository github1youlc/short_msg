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



pthread_mutex_t con_user_mutex;
MYSQL * con_user;



// handle register functions 
request_res * handle_logoff(cmd_logoff * logoff_info)
{
	request_res * res = new request_res;	// eventual result
	int tmp_res;         // store result every step

	// insert user info
	pthread_mutex_lock(&con_user_mutex);
	tmp_res = user_logoff(logoff_info, con_user);
	pthread_mutex_unlock(&con_user_mutex);

	if (tmp_res == -1 )
	{
		res->result = -1;
		res->msg = "mysql operation failed";
		return res;
	}else if(tmp_res == 0){
		res->result = 0;
		res->msg = "Invalid Param";
		return res;
	}else{
		res->result = 1;
		res->msg = "Logoff Succeed";
		return res;
	}
	return res;
}



void *recv_data(void *fd)
{
	int client_sockfd;
	client_sockfd=*((int*)fd);

	cmd_logoff * logoff_info;	
	request_res *res;
	string logff_json= get_cmd_from_sockfd(client_sockfd);
	printf("cmd: %s\n", logff_json.c_str());
	if (logff_json.size() == 0)
	{
		res = new request_res;
		res->result = -2;
		res->msg = "socket recv data error";
		thread_response_client(fd, client_sockfd, res);
	}
	logoff_info = get_logoff_info(logff_json);
	if (logoff_info == NULL) // the json command is not valid
	{
		res = new request_res;
		res->result = -2;
		res->msg = logff_json + ", Not Invalid Json Commond";
		thread_response_client(fd, client_sockfd, res);
	}
	res = handle_logoff(logoff_info);
	delete logoff_info; // free reg_info
	thread_response_client(fd, client_sockfd, res);
	return NULL;
}

int main()
{
	// initialize all database connections
	con_user = mysql_connect_db("127.0.0.1", "root", "123456", "user");

	int server_fd;
	int backlog = 40;

	// create daemon process
	//if(daemon(0,0) == -1)
	//	ERR_EXIT("daemon error");


	pthread_mutex_init(&con_user_mutex, NULL);

	server_fd = createServerSocket(SERVICE_LOGOUT_PORT, backlog);
	if (server_fd == -1)
	{
		printf("Error: create server socket\n");
		return 0;
	}
	while(1){
		int * client_fd = (int *)malloc(sizeof(int));
		printf(">>> SERVICE_LOGOUT, warting for connection\n");
		if((*client_fd =acceptClientSocket(server_fd)) == -1)
		{
			perror("socket accept");
			continue;
		}
		printf(">>> SERVICE_LOGOUT, new connection :%d\n", *client_fd);
		pthread_t thread;
		if(pthread_create(&thread, NULL, recv_data, client_fd)){
			perror("pthread create");
			continue;
		}
	}
	return 0;
}
#endif