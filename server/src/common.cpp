/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	TODO: common interface used by other codes
*/
/************************************************************************/

#ifndef SHORT_MSG_COMMON_CPP
#define SHORT_MSG_COMMON_CPP

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>

#include "common.h"
#include "json_helper.h"
#include "exten_def.h"


/************************************************************************/
/* get json command from socket                                   */
/************************************************************************/
string get_cmd_from_sockfd(int fd){
	char char_recv[MAX_BUFF_SIZE];//store data
	char tmp[RECV_ONCE_BUFF_SIZE];
	int l_count = 0; // count of '{'
	int r_count = 0; // count of '}'
	int zero_loop_count = 0; //count of not receiving data
	bool first_recv = true;
	while(true)
	{
		int byte_recv = 0;
		if((byte_recv=recv(fd, tmp, RECV_ONCE_BUFF_SIZE, 0))==-1)
		{
			perror("recv");
			return "";
		}
		//printf("%s\n", tmp);
		int len = strlen(tmp);

		if (byte_recv == 0)
		{
			zero_loop_count ++;
		}
		
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
		
		if (first_recv)
		{
			strcpy(char_recv, tmp);
		}else{
			strcat(char_recv, tmp);
		}

		for (int i=0;i<len;i++)
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
	send(client_sockfd, res_json.c_str(), MAX_BUFF_SIZE, 0);
	close(client_sockfd);
	pthread_exit(NULL);
}

void thread_response_client(void * fd, int client_sockfd, user_friends * res)
{
	string res_json = format_friends_json(res);
	delete res;	       // free the memory
	free(fd);
	send(client_sockfd, res_json.c_str(), MAX_BUFF_SIZE, 0);
	close(client_sockfd);
	pthread_exit(NULL);
}


void thread_response_client(void * fd, int client_sockfd, user_msgs * res)
{
	print_userMsgs(res);
	string res_json = format_msgs_json(res);
	printf("%s\n",res_json.c_str());
	free_userMsgs_struct(res);	       // free the memory
	free(fd);
	send(client_sockfd, res_json.c_str(), MAX_BUFF_SIZE, 0);
	close(client_sockfd);
	pthread_exit(NULL);
}

void thread_response_client(void * fd, int client_sockfd, string res_json)
{
	free(fd);
	send(client_sockfd, res_json.c_str(), MAX_BUFF_SIZE, 0);
	close(client_sockfd);
	pthread_exit(NULL);
}

void free_userMsgs_struct(user_msgs * all_msgs){
	vector<user_msg *>::iterator iter_one_person;
	vector<msg *>::iterator iter_one_msg;
	for (iter_one_person=all_msgs->user_msg.begin(); iter_one_person != all_msgs->user_msg.end(); iter_one_person++)
	{
		for (iter_one_msg=(*iter_one_person)->msgs.begin(); iter_one_msg != (*iter_one_person)->msgs.end(); iter_one_msg ++)
		{
			delete (*iter_one_msg);
		}
		delete (*iter_one_person);
	}
	delete all_msgs;
}


void print_userMsgs(user_msgs * all_msgs){
	if (all_msgs->count_msg == 0)
	{
		return;
	}
	vector<user_msg *>::iterator iter_one_person;
	vector<msg *>::iterator iter_one_msg;
	printf("\tTotal msgs: %d, from: %d persons\n", all_msgs->count_msg, all_msgs->count_person);
	for (iter_one_person=all_msgs->user_msg.begin(); iter_one_person != all_msgs->user_msg.end(); iter_one_person++)
	{
		printf("\t\tFrom: %s, count: %d\n",(*iter_one_person)->from.c_str(), (*iter_one_person)->count);
		for (iter_one_msg=(*iter_one_person)->msgs.begin(); iter_one_msg != (*iter_one_person)->msgs.end(); iter_one_msg ++)
		{
			printf("\t\t\ttime: %s\n\t\t\tmsg: %s\n", (*iter_one_msg)->time.c_str(), (*iter_one_msg)->data.c_str());
		}
		printf("\n");
	}
	printf("\n\n");
}

void print_userFriends(user_friends * all_friends){
	if (all_friends->count == 0)
	{
		return;
	}
	printf("\t All friends: %d\n", all_friends->count);
	vector<string>::iterator iter_friend;
	for (iter_friend = all_friends->friends.begin(); iter_friend != all_friends->friends.end(); iter_friend ++)
	{
		printf("\t\t%s\n", (*iter_friend).c_str());
	}
	printf("\n\n");
}

#endif