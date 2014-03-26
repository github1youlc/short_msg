/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	Todo: define all the cmd struct
*/
/************************************************************************/

#ifndef SHORT_MSG_CMD_H
#define SHORT_MSG_CMD_H

#include <vector>
#include <string>
using std::string;
using std::vector;


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




struct cmd_receiveMsg{
	string userID;
};

struct cmd_sendMsg{
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

struct msg{
	string data;
	string time;

};

struct user_msg{
	string from;
	bool is_read;
	int count;
	vector<msg *> msgs;
};

struct user_msgs{
	int count_msg;
	int count_person;
	vector<struct user_msg *> user_msg;
};

struct cmd_addFriend{
	string from;
	string to;
	string info;
};


struct cmd_confirmAdd {
	string from;
	string me;
	int is_accept;
};


struct cmd_getFriends{
	string me;
};


struct user_friends{
	int count;
	vector<string> friends;
};
#endif


