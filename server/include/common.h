/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	TODO: common interface used by other codes
*/
/************************************************************************/

#ifndef SHORT_MSG_COMMON_H
#define SHORT_MSG_COMMON_H

#include "cmd.h"

/************************************************************************/
/* get json command from socket                                   */
/************************************************************************/
string get_cmd_from_sockfd(int fd);

/************************************************************************/
/* 
	TODO: when the thread handling cliend socket is going to response to 
		server, do some clean and send back response;
*/
/************************************************************************/
void thread_response_client(void * fd, int client_sockfd, request_res * res);


#endif