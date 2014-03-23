#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "json_helper.h"

void request_res_test();


int main()
{
	string test_login = "{\"action\": \"register\", \"username\":\"user1\", \"password\": \"pwd1\", \"email\": \"xxx@xx.xx\", \"phone\": \"1234\"}";
	cmd_register * reg_info = get_register_info(test_login);
	string test_login2 = format_register_json(reg_info);
	printf("%s\n", test_login2.c_str());
	cmd_register * reg_info2 = get_register_info(test_login2);

	delete reg_info;
	delete reg_info2;


	request_res_test();
	return 0;
}

void request_res_test()
{
	request_res * res = new request_res;
	res->result = -1;
	res->msg = "test";
	string json_str = format_res_json(res);
	printf("%s", json_str.c_str());

	request_res* res1 = get_request_res(json_str);
	printf("%d ,%s",res1->result, res1->msg.c_str());
}