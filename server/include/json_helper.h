/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	Todo: interface file of json_helper.cpp
	Note: using boost library 
*/
/************************************************************************/
#include <string>
using std::string;


#include "cmd.h"

// get command type
string get_cmd_type(string json_str);


/************************************************************************/
/* 
	TODO: Translate json format string to defined struct 
*/
/************************************************************************/
cmd_register * get_register_info(string json_str);
cmd_login* get_login_info(string json_str);
cmd_logoff * get_logoff_info(string json_str);
request_res * get_request_res(string json_str);
user_msgs * get_user_msg(string json_str);
cmd_addFriend * get_addFriend_info(string json_str);
cmd_confirmAdd * get_confirmAdd_info(string json_str);
cmd_getFriends * get_getFriends_info(string json_str);
user_friends * get_user_friends(string json_str);
cmd_receiveMsg * get_receiveMsg_info(string json_str);



/************************************************************************/
/* 
	TODO: Translate defined struct to json string
*/
/************************************************************************/
string format_register_json(cmd_register* reg_info);
string format_login_json(cmd_login *login_info);
string format_logoff_json(cmd_logoff *logoff_info);
string format_res_json(request_res * res);
string format_msgs_json(user_msgs * all_msgs);

string format_sendMsg_json(cmd_sendMsg * sendMsg_info);
string format_receiveMsg_json(cmd_receiveMsg * receiveMsg_info);

string format_addFriend_json(cmd_addFriend * addFriend_info);
string format_confirmAdd_json(cmd_confirmAdd *confirmAdd_info);
string format_getFriends_json(cmd_getFriends * getFriends_info);
string format_friends_json(user_friends * fs);

