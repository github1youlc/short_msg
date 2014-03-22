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
using namespace boost::property_tree;

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

cmd_register * get_register_info(string json_str){
	cmd_register * reg_info = new cmd_register;
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
		reg_info->user_id = pt.get<string>("username");
		reg_info->password = pt.get<string>("password");
		reg_info->email = pt.get<string>("email", "");
		reg_info->phone = pt.get<string>("phone", "");
		reg_info->QQ = pt.get<string>("QQ", "");
	}  
	catch (ptree_error & e)  
	{
		printf("get information error, json_str: %s\n", json_str.c_str());
		return  NULL;
	} 
	return reg_info;
}


// parsing json text to get the login infomation
cmd_login* get_login_info(string json_str){
	cmd_login * login_info = new cmd_login;
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
 

//Format json string from cmd_register struct, the returned string can be send through socket
string format_register_json(cmd_register* reg_info){
	string reg_json;
	reg_json = reg_json + "{\"action\":\"register\"" + 
		",\"username\":" + "\"" + reg_info->user_id + "\"" +
		",\"password\":" + "\"" + reg_info->password+ "\"";

	if (reg_info->email.size() != 0)
	{
		reg_json = reg_json + ",\"email\":" + "\"" + reg_info->email+ "\"";
	}
	if (reg_info ->phone.size() != 0)
	{
		reg_json = reg_json + ",\"phone\":" + "\"" + reg_info->phone+ "\"";
	}
	if (reg_info->QQ.size() != 0)
	{
		reg_json = reg_json + ",\"QQ\":" + "\"" + reg_info->QQ+ "\"";
	}

	reg_json = reg_json + "}";

	return reg_json;
}

//Fromat json string from cmd_login
string format_login_json(cmd_login * cmd_login){
	string login_json;
	login_json = login_json + "{\"action\":\"login\"" + 
		",\"username\":" + "\"" + cmd_login->user_id + "\"" +
		",\"password\":" + "\"" + cmd_login->password+ "\"}";
	return login_json;
}

string format_res_json(request_res * res){
	string res_json;
	char int_tmp[20];
	sprintf(int_tmp, "%d", res->result);
	string result_str = int_tmp;
	res_json = res_json + 
		"{\"result:\":" + result_str +
		",\"msg\":" + "\"" + res->msg+ "\"}";
	return res_json;
}

