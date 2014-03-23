/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	TODO: common interface used by other codes
*/
/************************************************************************/

#ifndef SHORT_MSG_COMMON_CPP
#define SHORT_MSG_COMMON_CPP

#include "common.h"
#include "json_helper.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>

/************************************************************************/
/* get json command from socket                                   */
/************************************************************************/
string get_cmd_from_sockfd(int fd){
	char char_recv[1024];//store data
	char tmp[100];
	int l_count = 0; // count of '{'
	int r_count = 0; // count of '}'
	int zero_loop_count = 0; //count of not receiving data
	bool first_recv = true;
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

		for (int i=0;i<byte_recv;i++)
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


/************************************************************************/
/* 
	TODO: when the thread handling cliend socket is going to response to 
		server, do some clean and send back response;
*/
/************************************************************************/
void thread_response_client(void * fd, int client_sockfd, request_res * res)
{
	string res_json = format_res_json(res);
	delete res;	       // free the memory
	free(fd);
	send(client_sockfd, res_json.c_str(), 4096, 0);
	close(client_sockfd);
	pthread_exit(NULL);
}

#endif