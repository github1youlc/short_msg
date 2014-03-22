/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	Todo: define all the cmd struct
*/
/************************************************************************/

#ifndef SHORT_MSG_CMD_H
#define SHORT_MSG_CMD_H

#include <string>
using std::string;


// register command
struct cmd_register{
	string user_id;
	string password;
	string phone;
	string email;
	string QQ;
};


// login command
struct cmd_login{
	string user_id;
	string password;
};


// logoff cmd
struct cmd_logoff{
	string user_id;
};

// cmd used in message system
// type 1: getMsg command
// type 2: sendMsg command
struct cmd_msg{
	short type;
};


struct cmd_getMsg{
	short type;
	string userID;
};

struct cmd_sendMsg{
	short type;
	string fromUserId;
	string toUserId;
	string message;

};

struct request_res{
	int result;
	string msg;

	request_res(int r, const char * m){
		result = r;
		msg = m;
	}
	request_res(){}
};

#endif
