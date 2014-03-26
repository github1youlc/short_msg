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


pthread_mutex_t log_mutex;
int log_fd;

pthread_mutex_t con_user_mutex;
pthread_mutex_t con_relation_mutex;
pthread_mutex_t con_msg_mutex;
MYSQL * con_user;
MYSQL * con_relation;
MYSQL * con_msg;


// handle register functions 
request_res * handle_register(cmd_register * reg_info)
{
	request_res * res = new request_res;	// eventual result
	int tmp_res;         // store result every step

	// insert user info
	pthread_mutex_lock(&con_user_mutex);
	tmp_res = insert_user_info(reg_info, con_user);
	pthread_mutex_unlock(&con_user_mutex);

	if (tmp_res == -1 )
	{
		res->result = -1;
		res->msg = "mysql operation failed";
		return res;
	}else if(tmp_res == 0){
		res->result = 0;
		res->msg = "Already Reigstered";
		pthread_mutex_lock(&con_relation_mutex);
		create_relation_table(reg_info->user_id, con_relation);
		pthread_mutex_unlock(&con_relation_mutex);
		pthread_mutex_lock(&con_msg_mutex);
		create_msg_table(reg_info->user_id, con_msg);
		pthread_mutex_unlock(&con_msg_mutex);
		return res;
	}
	
	// create user relation database
	pthread_mutex_lock(&con_relation_mutex);
	tmp_res = create_relation_table(reg_info->user_id, con_relation);
	pthread_mutex_unlock(&con_relation_mutex);

	if (tmp_res == -1)
	{
		res->result = -1;
		res->msg = "mysql operation failed";
		return res;
	}


	// create user msg database
	pthread_mutex_lock(&con_msg_mutex);
	tmp_res = create_msg_table(reg_info->user_id, con_msg);
	pthread_mutex_unlock(&con_msg_mutex);

	if (tmp_res == -1)
	{
		res->result = -1;
		res->msg = "mysql operation failed";
		return res;
	}

	res->result = 1;
	res->msg = "Register Succeed";
	return res;
}

	

void *recv_data(void *fd)
{
	int client_sockfd;
	client_sockfd=*((int*)fd);
	
	cmd_register *reg_info;	
	request_res *res;
	string reg_json= get_cmd_from_sockfd(client_sockfd);
	printf("cmd: %s\n", reg_json.c_str());
	if (reg_json.size() == 0)
	{
		res = new request_res;
		res->result = -2;
		res->msg = "socket recv data error";
		thread_response_client(fd, client_sockfd, res);
	}
	reg_info = get_register_info(reg_json);
	if (reg_info == NULL) // the json command is not valid
	{
		res = new request_res;
		res->result = -2;
		res->msg = reg_json + ", not invalid json commond";
		thread_response_client(fd, client_sockfd, res);
	}
	res = handle_register(reg_info);
	delete reg_info; // free reg_info
	thread_response_client(fd, client_sockfd, res);
	return NULL;
}

int main()
{
	// initialize all database connections
	con_user = mysql_connect_db("127.0.0.1", "root", "123456", "user");
	con_relation = mysql_connect_db("127.0.0.1", "root", "123456", "relation");
	con_msg = mysql_connect_db("127.0.0.1", "root", "123456", "msg");

	int server_fd;
	int backlog = 40;

	// create daemon process
	//if(daemon(0,0) == -1)
	//	ERR_EXIT("daemon error");

	//initialize the log file mutex
	pthread_mutex_init(&log_mutex, NULL);
	pthread_mutex_init(&con_user_mutex, NULL);
	pthread_mutex_init(&con_relation_mutex, NULL);
	pthread_mutex_init(&con_msg_mutex, NULL);
	log_fd = open("/var/log/tencent/register.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
	server_fd = createServerSocket(SERVICE_REGISTER_PORT, backlog);
	if (server_fd == -1)
	{
		printf("Error: create server socket\n");
		return 0;
	}
	while(1){
		int * client_fd = (int *)malloc(sizeof(int));
		printf("warting for connection\n");
		if((*client_fd =acceptClientSocket(server_fd)) == -1)
		{
			perror("socket accept");
			continue;
		}
		printf("new connection :%d\n", *client_fd);
		pthread_t thread;
		if(pthread_create(&thread, NULL, recv_data, client_fd)){
			perror("pthread create");
			string err_str = "thread create error\n";
			pthread_mutex_lock(&log_mutex);
			write(log_fd, err_str.c_str(), err_str.size());
			pthread_mutex_unlock(&log_mutex);
			continue;
		}
	}
	return 0;
}
#endif