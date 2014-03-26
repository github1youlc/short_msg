#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <stdbool.h>
#include "../include/socketConnect.h"


#include "exten_def.h"

char * get_cmd_from_sockfd_c(int fd)
{
	//char * result_recv;
	char char_recv[MAX_BUFF_SIZE];//store data
	char tmp[RECV_ONCE_BUFF_SIZE];
	int l_count = 0; // count of '{'
	int r_count = 0; // count of '}'
	int zero_loop_count = 0; //count of not receiving data
	int first_recv = 1;
	int i;
	int right_bracket_pos = 0;
	while(true)
	{
		int byte_recv = 0;
		//printf("zero_count: %d\n", zero_loop_count);
		if((byte_recv=recv(fd, tmp, RECV_ONCE_BUFF_SIZE, 0))==-1)
		{
			perror("recv");
			return "";
		}

		
		if (byte_recv == 0)
		{
			zero_loop_count ++;
		}

		int len = strlen(tmp);
		//tmp[len] = '\0';
		//printf("recv :%s len:%d zero_count:%d\n ", tmp, len, zero_loop_count);

		if (zero_loop_count > 1000)
		{
			printf("No json command com, now close the socket\n");
			return "";
		}

		if (len > 4096)
		{
			printf("Not json command\n");
			return "";
		}

		if (first_recv == 1)
		{
			strcpy(char_recv, tmp);
			first_recv = 0;
		}else{
			strcat(char_recv, tmp);
		}
		//printf("char_recv: %s\n", char_recv);
		for (i=0;i<len;i++)
		{
			//printf("i= %d,%c\n", i,tmp[i]);
			if (tmp[i] == '{')
			{
				l_count++;
			}
			if (tmp[i] == '}')
			{
				right_bracket_pos = i;
				r_count ++;
			}
		}
		//printf("l_count: %d, r_count:%d", l_count, r_count);
		if (l_count == r_count && l_count !=0)
		{
			//printf("here\n");
			
			//char_recv[++right_bracket_pos] = '\0';
			break;
		}
	}
	//printf("%s", char_recv);
	//result_recv = new char [strlen(char_recv)];
	//strcpy(result_recv, char_recv);
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

char * send_buff(char const *ip, int port,char const *buff,int len)
{
	int sockfd;
	struct sockaddr_in server_address;

	if(-1==(sockfd=socket(AF_INET,SOCK_STREAM,0)))
	{   
		printf("socket() error\n");
		return "";
	}   
	memset(&server_address,0,sizeof(server_address));
	server_address.sin_family=AF_INET;
	server_address.sin_port=htons(port);
	if(-1==inet_pton(AF_INET,ip,&(server_address.sin_addr)))
	{   
		printf("inet_pton() error for %s\n",ip);
		return "";
	}   

	if(connect(sockfd,(struct sockaddr*)&server_address,sizeof(server_address))<0)
	{   
		printf("connect error\n");
		return "";
	}   
	//printf("socket:%d ,request to send: %s\n",sockfd, buff);
	if(send(sockfd,buff,len,0)<=0)
	{   
		printf("send_buff, send() error\n");
		return "";
	}
	//printf("socket:%d ,request sended: %s\n",sockfd, buff);
	char * json_res = get_cmd_from_sockfd_c(sockfd);
	//printf("result: %s\n", json_res);
	close(sockfd);
	return json_res;
}





