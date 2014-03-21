#include "json_helper.h"

int main()
{
	//string test_login = "{action: register, username:user1, password: pwd1, email: xxx@xx.xx, phone: 1234, QQ: 1234}";
	//string test_login = "{"action": "register", "username":"user1", "password": "pwd1", "email": "xxx@xx.xx", "phone": "1234", "QQ": "1234"}";

	string test_login = "{\"action\": \"register\", \"username\":\"user1\", \"password\": \"pwd1\", \"email\": \"xxx@xx.xx\", \"phone\": \"1234\"}";
	cmd_register * reg_info = get_register_info(test_login);
	return 0;
}