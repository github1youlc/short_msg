/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	TODO: Get client request and dispthch request to differen service server,
		after receving response from service server, send back reponse to client
*/
/************************************************************************/


#ifndef SHORT_MSG_ACCESS_LAYER_CPP
#define SHORT_MSG_ACCESS_LAYER_CPP

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
#include <boost/algorithm/string.hpp>
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
pthread_mutex_t con_relation_mutex;
pthread_mutex_t con_msg_mutex;
MYSQL * con_user;
MYSQL * con_relation;
MYSQL * con_msg;


// handle register functions 
string handle_request(string json_request)
{
	string cmd_type = get_cmd_type(json_request);
	boost::to_upper(cmd_type);
	string res_json;
	if (cmd_type == "REGISTER")
	{
		res_json = send_buff(SERVICE_REIGSTER_IP, SERVICE_REGISTER_PORT, json_request.c_str(), MAX_BUFF_SIZE);
	}else if(cmd_type == "LOGIN"){
		res_json = send_buff(SERVICE_LOGIN_IP, SERVICE_LOGIN_PORT, json_request.c_str(), MAX_BUFF_SIZE);
	}else if(cmd_type == "LOGOFF"){
		res_json = send_buff(SERVICE_LOGOUT_IP, SERVICE_LOGOUT_PORT, json_request.c_str(), MAX_BUFF_SIZE);
	}else if(cmd_type == "RECEIVEMSG"){
		res_json = send_buff(SERVICE_RECV_MSG_IP, SERVICE_RECV_MSG_PORT, json_request.c_str(), MAX_BUFF_SIZE);
	}else if(cmd_type == "SENDMSG"){
		res_json = send_buff(SERVICE_SEND_MSG_IP, SERVICE_SEND_MSG_PORT, json_request.c_str(), MAX_BUFF_SIZE);
	}else if(cmd_type == "ADDFRIEND"){
		res_json = send_buff(SERVICE_ADD_FRIEND_IP, SERVICE_ADD_FRIEND_PORT, json_request.c_str(), MAX_BUFF_SIZE);
	}else if(cmd_type == "CONFIRMADD"){
		res_json = send_buff(SERVICE_CONFIRM_ADD_FRIEND_IP, SERVICE_CONFIRM_ADD_FRIEND_PORT, json_request.c_str(), MAX_BUFF_SIZE);
	}else if (cmd_type == "GETFRIENDS"){
		res_json = send_buff(SERVICE_GET_FRIENDS_IP, SERVICE_GET_FRIENDS_PORT, json_request.c_str(), MAX_BUFF_SIZE);
	}
	else{
		res_json = "{\"result\": -1, \"msg\": \"can not get cmd type\"}";
	}
	if (res_json.size() == 0)
	{
		res_json = "{\"result\": -1, \"msg\": \"cannot get corresponding service\"}";
	}
	return res_json;
}



void *recv_data(void *fd)
{
	int client_sockfd;
	client_sockfd=*((int*)fd);
	request_res * res;
	string res_json;
	string request_json = get_cmd_from_sockfd(client_sockfd);
	printf("cmd: %s\n", request_json.c_str());
	if (request_json.size() == 0)
	{
		res = new request_res;
		res->result = -2;
		res->msg = "socket recv data error";
		thread_response_client(fd, client_sockfd, res);
	}
	res_json = handle_request(request_json);
	thread_response_client(fd, client_sockfd, res_json);
	return NULL;
}

int main()
{
	int server_fd;
	int backlog = 40;

	// create daemon process
	//if(daemon(0,0) == -1)
	//	ERR_EXIT("daemon error");

	server_fd = createServerSocket(ACCESS_LAYER_PORT, backlog);
	if (server_fd == -1)
	{
		printf("Error: create server socket\n");
		return 0;
	}
	while(1){
		int * client_fd = (int *)malloc(sizeof(int));
		printf(">>> Access Layer: warting for connection\n");
		if((*client_fd =acceptClientSocket(server_fd)) == -1)
		{
			perror("socket accept");
			continue;
		}
		printf(">>> Access Layer, new connection :%d\n", *client_fd);
		pthread_t thread;
		if(pthread_create(&thread, NULL, recv_data, client_fd)){
			perror("create thread\n");
			continue;
		}
	}
	return 0;
}
#endif