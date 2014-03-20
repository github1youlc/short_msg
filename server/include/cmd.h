/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	Todo: define all the cmd struct
*/
/************************************************************************/

#ifndef SHORT_MSG_CMD_H
#define SHORT_MSG_CMD_H

#include <string.h>
#include <string>
using std::string;


// login cmd
struct cmd_login{
	string user_id;
	string password;
	string phone;
	string email;
	string QQ;
};


// logoff cmd
struct cmd_logoff{
	string user_id;
};

#endif