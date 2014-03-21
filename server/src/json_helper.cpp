/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	Todo: helper functions when handling json text
	Note: using boost library 
*/
/************************************************************************/


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <stdio.h>

#include "json_helper.h"


// get command type
string get_cmd_type(string json_str){
	std::stringstream ss(json_str);
	ptree pt;
	string cmd_type;
	try
	{
		read_json(ss, pt);
		cmd_type = pt.get<string>("action");
	}
	catch (ptree_error &e)
	{
		printf("get command type error, json_str: %s\n", json_str.c_str());
		return string("unknown");
	}
	return cmd_type;
}


// parsing json text to get the login infomation
cmd_login* get_login_info(string json_str){
	cmd_login * login_info;
	std::stringstream ss(json_str);
	ptree pt;
	string cmd_type;
	try
	{
		read_json(ss, pt);
	}
	catch (ptree_error &e)
	{
		printf("read json error, json_str: %s\n", json_str.c_str());
		return NULL;
	}
	

	try{  
		login_info->user_id = pt.get<string>("username");
		login_info->password = pt.get<string>("password");
	}  
	catch (ptree_error & e)  
	{  
		return  NULL;
	} 
	return login_info;
}
 
