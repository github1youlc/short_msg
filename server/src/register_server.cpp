#ifndef SHORT_MSG_REGISTER_SERVER_CPP
#define SHORT_MSG_REGISTER_SERVER_CPP

#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "cmd.h"
#include "mysql_helper.h"


extern "C" {
#include "socketConnect.h"
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include <string>
using std::string;

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

void *handle_register(void *fd)
{
	int client_sockfd;
	int byte;
	char char_recv[100];//´æ·ÅÊý¾Ý
	client_sockfd=*((int*)fd);
	for(;;)
	{
		if((byte=recv(client_sockfd,char_recv,100,0))==-1)
		{
			perror("recv");
			pthread_exit(NULL);
		}
		pthread_mutex_lock(&log_mutex);
		write(log_fd, char_recv, (size_t)byte);//write data to log_dir
		pthread_mutex_unlock(&log_mutex);
		if(strcmp(char_recv, "exit")==0)//recv exit, client disconnects by itself
			break;	
	}
	free(fd);
	close(client_sockfd);
	pthread_exit(NULL);
}

int main()
{
	int server_fd;
	unsigned int port = 0x8881;
	int backlog = 40;

	// create daemon process
	//if(daemon(0,0) == -1)
	//	ERR_EXIT("daemon error");

	//initialize the log file mutex
	pthread_mutex_init(&log_mutex, NULL);
	log_fd = open("/var/log/tencent/register.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
	server_fd = createServerSocket(port, backlog);
	while(1){
		int * client_fd = (int *)malloc(sizeof(int));
		if((*client_fd =acceptClientSocket(server_fd)) = -1)
		{
			perror("socket accept");
			continue;
		}
		pthread_t thread;
		if(pthread_create(&thread, NULL, handle_register, client_fd)){
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