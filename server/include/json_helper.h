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

cmd_register * get_register_info(string json_str);


cmd_login* get_login_info(string json_str);



string format_register_json(cmd_register* reg_info);


string format_login_json(cmd_login *login_info);


string format_res_json(request_res * res);
