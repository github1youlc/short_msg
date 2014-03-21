#ifdef SHORT_MSG_REGISTER_SERVER_CPP
#define SHORT_MSG_REGISTER_SERVER_CPP


#include "cmd.h";
#include "mysql_helper.h"

// from server.c 
exten "C" {
	int createServerSocket(int port, int backlog);
	int acceptClientSocket(int serverSocket);
};

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>


//define error exit
#define ERR_EXIT(m) \
do\
{\
	perror(m);\
	exit(EXIT_FAILURE);\
}\
while (0);\

void *handle_register(void *fd)
{
	int client_sockfd;
	int i,byte;
	char char_recv[100];//存放数据
	client_sockfd=*((int*)fd);
	for(;;)
	{
		if((byte=recv(client_sockfd,char_recv,100,0))==-1)
		{
			perror("recv");
			pthread_exit(NULL);
		}
		if(strcmp(char_recv, "exit")==0)//接受到exit时，跳出循环
			break;
		printf("receive from client is %s/n",char_recv);//打印收到的数据
	}
	free(fd);
	close(client_sockfd);
	pthread_exit(NULL);
}

int main(void)
{
	int server_fd;
	unsigned int port = 0x8881;
	int backlog = 40;
	// create daemon process
	if(daemon(0,0) == -1)
		ERR_EXIT("daemon error");
	int log_fd = open("/var/log/tencent/register.log",O_WRONLY|O_CREAT|O_APPEND,0644);
	fd = createServerSocket(port, backlog);
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
			send(client_fd, "-1",1, 0);
			continue;
		}
	}
	return 0;
}

bool register(cmd_register * reg_info){
	// check whether the id is exist


	// check validation of email

	// after checking, handle the register:
	//		1. insert info to user.users
	//		2. create table of relation for the user in relation db
	//		3. create table of msg for the user in msg db

}


int main()
{

}

#endif