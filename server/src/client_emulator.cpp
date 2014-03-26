#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <boost/algorithm/string.hpp>
using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

#include "socketConnect.h"

#ifdef	__cplusplus
}
#endif

#include "json_helper.h"
#include "exten_def.h"
#include "common.h"

map<string, int> cmd_map;

void init_cmd_map();

void handle_cmd(vector<string> tokens);



string my_user_id;


void * msg_push(void * void_type)
{
	cmd_receiveMsg * receiveMsg_info;
	string receiveMsg_json;
	string json_res;
	request_res * res;
	user_msgs * push_all_msgs;
	while(true){
		if (my_user_id != "")
		{
			receiveMsg_info = new cmd_receiveMsg;
			receiveMsg_info->userID = my_user_id;
			receiveMsg_json = format_receiveMsg_json(receiveMsg_info);
			json_res = send_buff(ACCESS_LAYER_IP, ACCESS_LAYER_PORT, receiveMsg_json.c_str(), MAX_BUFF_SIZE);
			delete receiveMsg_info;
			push_all_msgs = get_user_msg(json_res);
			if (push_all_msgs != NULL && push_all_msgs->count_msg != 0)
			{
				print_userMsgs(push_all_msgs);
				free_userMsgs_struct(push_all_msgs);
			}
		}
		sleep(5);
	}
	return NULL;
}

int main(){
	init_cmd_map();
	string line;
	pthread_t thread;
	if(pthread_create(&thread, NULL, msg_push, NULL)){
		perror("create thread\n");
		return 1;
	}
	printf("------------Welcome To Short Message!-------------\n");
	printf(">>");
	fflush(stdout);
	while (getline(std::cin, line))
	{
		string str;
		vector<string> tokens;
		istringstream stream(line);
		while(stream>>str)
			tokens.push_back(str);

		handle_cmd(tokens);
		tokens.clear();
		printf(">>");
		fflush(stdout);
	}
}

void init_cmd_map(){
	// FOR REGISTER COMMAND
	cmd_map.insert(std::pair<string, int>("REGISTER", 1));
	cmd_map.insert(std::pair<string, int>("-QQ", 11));
	cmd_map.insert(std::pair<string, int>("-email", 12));
	cmd_map.insert(std::pair<string, int>("-phone", 13));
}

void handle_cmd(vector<string> tokens){
	int rem_para_count  = tokens.size();
	if (rem_para_count < 1)
	{
		return;
	}
	string cmd_type = tokens.at(0);
	boost::to_upper(cmd_type);
	string res_json;
	string username, password, QQ, email, phone;
	string reg_json;
	string cmd_json;
	request_res * res_struct;
	cmd_register *reg_info;
	cmd_login * login_info;
	cmd_logoff * logoff_info;
	cmd_sendMsg * sendMsg_info;
	cmd_receiveMsg * receiveMsg_info;
	cmd_addFriend * addFriend_info;
	cmd_confirmAdd * confirmAdd_info;
	cmd_getFriends * getFriends_info;
	user_friends * all_friends;
	user_msgs * all_msgs;
	if (cmd_type == "REGISTER")
	{

		rem_para_count -= 1;
		if (rem_para_count < 2)
		{
			printf("\tIncorrect command, using: register <username> <password> [-QQ QQValue] [-email emailValue] [-phone, phoneValue]\n");
			return;
		}else
		{
			username = tokens.at(1);
			password = tokens.at(2);
			rem_para_count -= 2;
		}
		while(rem_para_count >= 2)
		{
			int cur_index = tokens.size() - rem_para_count;
			switch(cmd_map[tokens.at(cur_index)]){
			case 11:
				QQ = tokens.at(cur_index+1);
				rem_para_count -= 2;
				break;

			case 12:
				email = tokens.at(cur_index+1);
				rem_para_count -= 2;
				break;

			case 13:
				phone = tokens.at(cur_index+1);
				rem_para_count -= 2;
				break;

			default:
				printf("\tIncorrect command, using: register <username> <password> [-QQ QQValue] [-email emailValue] [-phone, phoneValue]\n");
				return;
			}
		}
		if (rem_para_count == 1) // wrong command format
		{
			printf("\tIncorrect command, using: register <username> <password> [-QQ QQValue] [-email emailValue] [-phone, phoneValue]\n");
			return;
		}
		reg_info = new cmd_register;
		reg_info->user_id = username;
		reg_info->password = password;
		reg_info->email = email;
		reg_info->phone = phone;
		reg_info->QQ = QQ;

		reg_json = format_register_json(reg_info);
		//printf("reg_json: %s\n", reg_json.c_str());
		delete reg_info;
		res_json = send_buff("127.0.0.1", ACCESS_LAYER_PORT, reg_json.c_str(), 4096);
		res_struct = get_request_res(res_json);
		printf("\t%s\n\n",res_struct->msg.c_str());
		delete res_struct;
	}else if (cmd_type == "LOGIN"){
		rem_para_count -= 1;
		if (rem_para_count == 2)
		{
			int cur_index = tokens.size() - rem_para_count;
			login_info = new cmd_login;
			login_info->user_id = tokens.at(cur_index);
			login_info->password = tokens.at(cur_index+1);
			cmd_json = format_login_json(login_info);
			delete login_info;
			res_json = send_buff("127.0.0.1", ACCESS_LAYER_PORT, cmd_json.c_str(), 4096);
			res_struct = get_request_res(res_json);
			if (res_struct->result == 1) //if login succeed, the save user_id
			{
				my_user_id = tokens.at(cur_index);
			}
			printf("\t%s\n\n",res_struct->msg.c_str());
			delete res_struct;
		}else{
			printf("\tIncorrect command, using: login <username> <password>\n");
		}
	}else if(cmd_type == "LOGOUT"){
		rem_para_count -= 1;
		if (rem_para_count == 1)
		{
			int cur_index = tokens.size() - rem_para_count;
			logoff_info = new cmd_logoff;
			logoff_info->user_id = tokens.at(cur_index);
			cmd_json = format_logoff_json(logoff_info);
			delete logoff_info;
			res_json = send_buff("127.0.0.1", ACCESS_LAYER_PORT, cmd_json.c_str(), 4096);
			res_struct = get_request_res(res_json);
			if (res_struct == NULL)
			{
				return;
			}
			if (res_struct->result == 1)
			{
				my_user_id = "";
			}
			printf("\t%s\n\n",res_struct->msg.c_str());
			delete res_struct;
		}
	}else if(cmd_type == "SEND"){
		if (my_user_id.size() == 0)
		{
			printf("\tplease login to send messagee to friend\n");
			return;
		}
		rem_para_count -= 1;
		if (rem_para_count == 2)
		{
			int cur_index = tokens.size() - rem_para_count;
			sendMsg_info = new cmd_sendMsg;
			sendMsg_info->fromUserId = my_user_id;
			sendMsg_info->toUserId = tokens.at(cur_index);
			sendMsg_info->message = tokens.at(cur_index+1);
			cmd_json = format_sendMsg_json(sendMsg_info);
			delete sendMsg_info;
			if (res_json.size() > RECV_ONCE_BUFF_SIZE)
			{
				printf("\tmessage to send is too long\n");
			}
			res_json = send_buff("127.0.0.1", ACCESS_LAYER_PORT, cmd_json.c_str(), 4096);
			res_struct = get_request_res(res_json);
			if (res_struct == NULL)
			{
				return;
			}
			printf("\t%s\n\n",res_struct->msg.c_str());
			delete res_struct;
		}
	}else if(cmd_type == "RECEIVE"){
		if (my_user_id.size() == 0)
		{
			printf("\tplease login to receive message\n");
			return;
		}
		rem_para_count -= 1;
		if (rem_para_count == 0)
		{
			receiveMsg_info = new cmd_receiveMsg;
			receiveMsg_info->userID = my_user_id;
			cmd_json = format_receiveMsg_json(receiveMsg_info);
			delete receiveMsg_info;
			res_json = send_buff("127.0.0.1", ACCESS_LAYER_PORT, cmd_json.c_str(), 4096);
			all_msgs = get_user_msg(res_json);
			if (all_msgs == NULL)
			{
				return;
			}
			print_userMsgs(all_msgs);
			free_userMsgs_struct(all_msgs);
		}else
		{
			printf("\tIncorrect Command, using: receiveMsg");
		}
	}else if(cmd_type == "ADD"){
		if (my_user_id.size() == 0)
		{
			printf("\tplease login before adding friend\n");
			return;
		}
		rem_para_count -= 1;
		if (rem_para_count == 2)
		{
			int cur_index = tokens.size() - rem_para_count;
			addFriend_info = new cmd_addFriend;
			addFriend_info->from = my_user_id;
			addFriend_info->to = tokens.at(cur_index);
			addFriend_info->info = tokens.at(cur_index+1);
			cmd_json = format_addFriend_json(addFriend_info);
			delete addFriend_info;
			res_json = send_buff("127.0.0.1", ACCESS_LAYER_PORT, cmd_json.c_str(), 4096);
			res_struct = get_request_res(res_json);
			if (res_struct == NULL)
			{
				return;
			}
			printf("\t%s\n\n",res_struct->msg.c_str());
			delete res_struct;
		}
	}else if (cmd_type == "ACCEPT" || (cmd_type == "REFUSE"))
	{
		if (my_user_id.size() == 0)
		{
			printf("\tplease login before handle adding friend request\n");
			return;
		}
		rem_para_count -= 1;
		if (rem_para_count == 1)
		{
			int cur_index = tokens.size() - rem_para_count;
			confirmAdd_info = new cmd_confirmAdd;
			confirmAdd_info->me = my_user_id;
			confirmAdd_info->from = tokens.at(cur_index);
			if (cmd_type == "ACCEPT")
			{
				confirmAdd_info->is_accept = 1;
			}else
				confirmAdd_info->is_accept = 0;

			cmd_json = format_confirmAdd_json(confirmAdd_info);
			delete confirmAdd_info;
			res_json = send_buff("127.0.0.1", ACCESS_LAYER_PORT, cmd_json.c_str(), 4096);
			res_struct = get_request_res(res_json);
			if (res_struct == NULL)
			{
				return;
			}
			printf("\t%s\n\n",res_struct->msg.c_str());
			delete res_struct;
		}else{
			printf("Incorrect command, using: ACCEPT/REFUSE <username>");
			return;
		}
	}else if(cmd_type == "FRIENDS"){
		if (my_user_id.size() == 0)
		{
			printf("\tplease login before list friends\n");
			return;
		}
		rem_para_count -= 1;
		if (rem_para_count == 0)
		{
			getFriends_info = new cmd_getFriends;
			getFriends_info->me = my_user_id;
			cmd_json = format_getFriends_json(getFriends_info);
			delete getFriends_info;
			res_json = send_buff("127.0.0.1", ACCESS_LAYER_PORT, cmd_json.c_str(), 4096);
			all_friends = get_user_friends(res_json);
			if (all_friends == NULL)
			{
				return;
			}
			print_userFriends(all_friends);
			delete all_friends;
		}else{
			printf("Incorrect command, using: getFriends");
			return;
		}
	}else if(cmd_type == "HELP"){
		printf("\tCommand List: (Ignore Character Case)\n");
		printf("\t\tregister <username> <password> [-QQ QQValue] [-email emailValue] [-phone, phoneValue]\n");
		printf("\t\tlogin <username> <password>\n");
		printf("\t\tlogout\n");
		printf("\t\taddfriend <username> <validation_ifno>\n");
		printf("\t\taccept <username>\n");
		printf("\t\trefuse <username>\n");
		printf("\t\tfriends\n");
		printf("\t\tsendmsg <username> <msg>\n");
		printf("\t\treceivemsg\n");
		printf("\t\thelp\n\n");
		return;
	}
	else{
		printf("\t\tIncorrect command, get all command, using: help \n");
		return;
	}
}
