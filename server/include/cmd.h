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

#endif