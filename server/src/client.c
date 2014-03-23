#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <stdbool.h>
#include "../include/socketConnect.h"


#define MAXLINE 4096

char * get_cmd_from_sockfd_c(int fd)
{
	char char_recv[1024];//store data
	char tmp[100];
	int l_count = 0; // count of '{'
	int r_count = 0; // count of '}'
	int zero_loop_count = 0; //count of not receiving data
	bool first_recv = true;
	int i;
	while(true)
	{
		int byte_recv;
		if((byte_recv=recv(fd,tmp,100,0))==-1)
		{
			perror("recv");
			return "";
		}

		if (byte_recv == 0)
		{
			zero_loop_count ++;
		}

		if (zero_loop_count > 1000)
		{
			printf("No json command com, now close the socket\n");
			return "";
		}

		if (strlen(char_recv) > 500)
		{
			printf("Not json command\n");
			return "";
		}

		if (first_recv)
		{
			strcpy(char_recv, tmp);
		}else{
			strcat(char_recv, tmp);
		}

		for (i=0;i<byte_recv;i++)
		{
			if (tmp[i] == '{')
			{
				l_count++;
			}
			if (tmp[i] == '}')
			{
				r_count++;
			}
		}

		if (l_count == r_count && l_count !=0)
		{
			break;
		}
	}
	return char_recv;
}

int createClientSocket(char * ip, int port) {
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1) {
		return -1;
	}
	struct sockaddr_in serverAddress;
	memset(&serverAddress,0, sizeof(serverAddress));
	int success = inet_pton(AF_INET, ip, &serverAddress.sin_addr);
	if (!success) {
		return -1;
	}
	serverAddress.sin_port = htons(port);
	serverAddress.sin_family=AF_INET;

	success = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if (success == -1) {
		return -1;
	}
	return clientSocket;
}

int send_buff(char const *ip, int port,char const *buff,int len)
{
	int sockfd;
	struct sockaddr_in server_address;

	if(-1==(sockfd=socket(AF_INET,SOCK_STREAM,0)))
	{   
		printf("socket() error\n");
		return 0;
	}   
	memset(&server_address,0,sizeof(server_address));
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(port);
	if(-1==inet_pton(AF_INET,ip,&(server_address.sin_addr)))
	{   
		printf("inet_pton() error for %s\n",ip);
		return 0;
	}   

	if(connect(sockfd,(struct sockaddr*)&server_address,sizeof(server_address))<0)
	{   
		printf("connect error\n");
		return 0;
	}   
	printf("socket:%d ,request to send: %s",sockfd, buff);
	if(send(sockfd,buff,len,0)<=0)
	{   
		printf("send_buff, send() error\n");
		return 0;
	}
	printf("socket:%d ,request sended: %s",sockfd, buff);
	char * json_res = get_cmd_from_sockfd_c(sockfd);
	printf("result: %s", json_res);
	close(sockfd);
	return 1;
}





