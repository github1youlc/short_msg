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

#define CLIENT_CONNECT_PORT 6000

map<string, int> cmd_map;

void init_cmd_map();

void handle_cmd(vector<string> tokens){
	int rem_para_count  = tokens.size();
	if (rem_para_count < 1)
	{
		return;
	}
	string cmd_type = tokens.at(0);
	boost::to_upper(cmd_type);
	string username, password, QQ, email, phone;
	string reg_json;
	cmd_register *reg_info;
	switch(cmd_map[cmd_type]){
		case 1:
			
			rem_para_count -= 1;
			if (rem_para_count < 2)
			{
				printf("\t\tIncorrect command, using: register <username> <password> [-QQ QQValue] [-email emailValue] [phone, phoneValue]");
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
					printf("\t\tIncorrect command, using: register <username> <password> [-QQ QQValue] [-email emailValue] [phone, phoneValue]");
					return;
				}
			}
			if (rem_para_count == 1) // wrong command format
			{
				printf("\t\tIncorrect command, using: register <username> <password> [-QQ QQValue] [-email emailValue] [phone, phoneValue]");
				return;
			}
			reg_info = new cmd_register;
			reg_info->user_id = username;
			reg_info->password = password;
			reg_info->email = email;
			reg_info->phone = phone;
			reg_info->QQ = QQ;
			
			reg_json = format_register_json(reg_info);
			printf("reg_json: %s", reg_json.c_str());
			delete reg_info;

			send_buff("127.0.0.1", CLIENT_CONNECT_PORT, reg_json.c_str(), 100);
			break;

		default:
			printf("\t\tIncorrect command, get all command, using: help \n");
			return;
	}
}


int main(){
	init_cmd_map();
	string line;
	while (getline(std::cin, line))
	{
		string str;
		vector<string> tokens;
		istringstream stream(line);
		while(stream>>str)
			tokens.push_back(str);

		handle_cmd(tokens);
		tokens.clear();
	}
	return 0;
}

void init_cmd_map(){
	// FOR REGISTER COMMAND
	cmd_map.insert(std::pair<string, int>("REGISTER", 1));
	cmd_map.insert(std::pair<string, int>("-QQ", 11));
	cmd_map.insert(std::pair<string, int>("-email", 12));
	cmd_map.insert(std::pair<string, int>("-phone", 13));
}