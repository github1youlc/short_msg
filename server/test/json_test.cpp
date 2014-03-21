#include "json_helper.h"
#include <stdio.h>

int main()
{
	//string test_login = "{action: register, username:user1, password: pwd1, email: xxx@xx.xx, phone: 1234, QQ: 1234}";
	//string test_login = "{"action": "register", "username":"user1", "password": "pwd1", "email": "xxx@xx.xx", "phone": "1234", "QQ": "1234"}";

	string test_login = "{\"action\": \"register\", \"username\":\"user1\", \"password\": \"pwd1\", \"email\": \"xxx@xx.xx\", \"phone\": \"1234\"}";
	cmd_register * reg_info = get_register_info(test_login);
	string test_login2 = format_register_json(reg_info);
	printf("%s\n", test_login2.c_str());
	cmd_register * reg_info2 = get_register_info(test_login2);
	return 0;
}