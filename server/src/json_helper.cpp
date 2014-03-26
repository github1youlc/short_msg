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

cmd_logoff* get_logoff_info(string json_str)
{
	cmd_logoff *logoff_info = new cmd_logoff;
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
		logoff_info->user_id = pt.get<string>("username");
	}  
	catch (ptree_error & e)  
	{  
		return  NULL;
	} 
	return logoff_info;
}

request_res * get_request_res(string json_str){
	request_res * res = new request_res;

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
		res->result = pt.get<int>("result", 0);
		res->msg = pt.get<string>("msg", "");
	}  
	catch (ptree_error & e)  
	{  
		return  NULL;
	}
	return res;
}


user_msgs * get_user_msg(string json_str)
{
	user_msgs * res = new user_msgs;

	std::stringstream ss(json_str);
	ptree pt;
	string cmd_type;
	try
	{
		read_json(ss, pt);
	}
	catch (ptree_error &e)
	{
		delete res;
		printf("read json error, json_str: %s\n", json_str.c_str());
		return NULL;
	}

	try{  
		ptree all_user_msgs_array = pt.get_child("msgs");
		res->count_msg = pt.get<int>("totalmsg");
		res->count_person = pt.get<int>("peernum");
		BOOST_FOREACH(ptree::value_type & v, all_user_msgs_array){
			user_msg * one_person_msgs = new user_msg;
			one_person_msgs->count = v.second.get<int>("msgnum");
			one_person_msgs->from = v.second.get<string>("peer");
			one_person_msgs->is_read = false;
			ptree one_person_msgs_array = v.second.get_child("data");
			BOOST_FOREACH(ptree::value_type & v_sub, one_person_msgs_array){
				msg * a_msg = new msg;
				a_msg->data = v_sub.second.get<string>("msg", "");
				a_msg->time = v_sub.second.get<string>("time", "");
				one_person_msgs->msgs.push_back(a_msg);
			}
			res->user_msg.push_back(one_person_msgs);
		}
	}  
	catch (ptree_error & e)  
	{
		delete res;
		return  NULL;
	}
	return res;
}

cmd_addFriend * get_addFriend_info(string json_str){
	cmd_addFriend * res = new cmd_addFriend;

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
		res->from = pt.get<string>("from");
		res->to = pt.get<string>("to");
		res->info = pt.get<string>("info");
	}  
	catch (ptree_error & e)  
	{  
		return  NULL;
	}
	return res;
}

cmd_confirmAdd * get_confirmAdd_info(string json_str){
	cmd_confirmAdd * res = new cmd_confirmAdd;

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
		res->from = pt.get<string>("from");
		res->me = pt.get<string>("me");
		res->is_accept = pt.get<int>("is_accept");
	}  
	catch (ptree_error & e)  
	{  
		return  NULL;
	}
	return res;
}

cmd_getFriends * get_getFriends_info(string json_str){
	cmd_getFriends * res = new cmd_getFriends;

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
		res->me = pt.get<string>("me");
	}  
	catch (ptree_error & e)  
	{  
		return  NULL;
	}
	return res;
}

user_friends * get_user_friends(string json_str){
	user_friends * res = new user_friends;

	std::stringstream ss(json_str);
	ptree pt;
	string cmd_type;
	try
	{
		read_json(ss, pt);
	}
	catch (ptree_error &e)
	{
		delete res;
		printf("read json error, json_str: %s\n", json_str.c_str());
		return NULL;
	}

	try{  
		ptree all_user_msgs_array = pt.get_child("friends");
		res->count = pt.get<int>("count");
		BOOST_FOREACH(ptree::value_type & v, all_user_msgs_array){
			res->friends.push_back(v.second.get<string>("name"));
		}
	}  
	catch (ptree_error & e)  
	{
		delete res;
		return  NULL;
	}

	return res;
}

cmd_receiveMsg * get_receiveMsg_info(string json_str){
	cmd_receiveMsg * res = new cmd_receiveMsg;

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
		res->userID = pt.get<string>("id");
	}  
	catch (ptree_error & e)  
	{  
		return  NULL;
	}
	return res;
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

string format_logoff_json(cmd_logoff * cmd_logoff){
	string logoff_json;
	logoff_json = logoff_json + "{\"action\":\"logoff\"" + 
		",\"username\":" + "\"" + cmd_logoff->user_id + "\"}"; 
	return logoff_json;
}

string format_res_json(request_res * res){
	string res_json;
	char int_tmp[20];
	sprintf(int_tmp, "%d", res->result);
	string result_str = int_tmp;
	res_json = res_json + 
		"{\"result\":" + result_str +
		",\"msg\":" + "\"" + res->msg+ "\"}";
	return res_json;
}

string format_msgs_json(user_msgs * all_msgs){
	int i;
	int j;
	string res_json;
	char count_all_msg[20];
	char count_one_person_msg[20];
	char count_persons[20];
	sprintf(count_all_msg, "%d", all_msgs->count_msg);
	res_json = res_json + 
		"{\"totalmsg\":" + count_all_msg +
		",\"msgs\":" + "[";
	int num_person = all_msgs->count_person;
	for (i=0;i<num_person;i++)
	{	
		if (i >= 1)
		{
			res_json = res_json + ",";
		}
		user_msg * tmp = all_msgs->user_msg.at(i);
		int one_person_cout = tmp->count;
		res_json = res_json + "{\"peer\":" + "\"" + tmp->from + "\""
			+",\"data\":[";
		for (j=0;j<one_person_cout;j++)
		{
			msg * tmp2 = tmp->msgs[j];
			if (j >= 1)
			{
				res_json = res_json + ",";
			}
			res_json = res_json + 
				"{\"msg\":" + "\"" + tmp2->data + "\"" +
				",\"time\":" + "\"" + tmp2->time + "\"}";
		}
		sprintf(count_one_person_msg, "%d", one_person_cout);
		res_json = res_json + "]" +
			",\"msgnum\":" + count_one_person_msg + "}";
	}
	sprintf(count_persons, "%d", num_person);
	res_json = res_json + "]" +
		",\"peernum\":" + count_persons + "}";

	return res_json;
}


string format_sendMsg_json(cmd_sendMsg * sendMsg_info){
	string res_json;
	res_json = res_json + "{\"action\":" + "\"" + "sendMsg" + "\""
		+ ",\"from\":" + "\"" + sendMsg_info->fromUserId + "\""
		+ ",\"to\":" + "\"" + sendMsg_info->toUserId + "\""
		+ ",\"msg\":" + "\"" + sendMsg_info->message + "\"}";
	return res_json;
}


string format_receiveMsg_json(cmd_receiveMsg * receiveMsg_info){
	string res_json;
	res_json = res_json + "{\"action\":" + "\"" + "receiveMsg" + "\""
		+",\"id\":" + "\"" + receiveMsg_info->userID + "\"}";
	return res_json;
}


string format_addFriend_json(cmd_addFriend * addFriend_info){
	string res_json;
	res_json = res_json + "{\"action\":" + "\"" + "addFriend" + "\""
		+ ",\"from\":" + "\"" + addFriend_info->from + "\""
		+ ",\"to\":" + "\"" + addFriend_info->to + "\""
		+ ",\"info\":" + "\"" + addFriend_info->info + "\"}";

	return res_json;
}

string format_confirmAdd_json(cmd_confirmAdd *confirmAdd_info){
	string res_json;
	char str_is_accept[20];
	sprintf(str_is_accept, "%d", confirmAdd_info->is_accept);
	res_json = res_json + "{\"action\":" + "\"" + "confirmAdd" + "\""
		+",\"from\":" + "\"" + confirmAdd_info->from + "\""
		+",\"me\":" + "\"" + confirmAdd_info->me + "\""
		+",\"is_accept\":" + str_is_accept + "}";
	return res_json;
}

string format_getFriends_json(cmd_getFriends * getFriends_info){
	string res_json;
	res_json = res_json + "{\"action\":" + "\"" + "getFriends" + "\""
		+",\"me\":" + "\"" + getFriends_info->me + "\"}";
	return res_json;
}


string format_friends_json(user_friends * fs){
	string res_json;
	char count_fs[20];
	sprintf(count_fs, "%d", fs->count);
	res_json = res_json + "{\"count\":" + count_fs + 
		",\"friends\":[";

	int i;
	for (i=0; i< fs->count;i++)
	{
		if (i > 0)
		{
			res_json = res_json + ",";
		}
		res_json = res_json + "{\"name\":" + "\"" + fs->friends.at(i) + "\"}";
	}

	res_json = res_json + "]}";
	return res_json;
}