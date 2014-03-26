#include "mysql_helper.h"
#include "cmd.h"
#include "json_helper.h"

#include "common.h"
#include <stdio.h>

void mysql_test_create_relation_talbe(MYSQL *con){
	create_relation_table("10000", con);
	
}

void test_insert_user_info(MYSQL * con){
	cmd_register * reg_info = new cmd_register;
	reg_info->user_id = "test1";
	reg_info->password = "pass1";
	reg_info->QQ = "12345";
	insert_user_info(reg_info, con);
}


void test_fetch_usr_msg(MYSQL * con)
{
	user_msgs * res = fetch_user_msgs("user2", con, 0);
	string res_js = format_msgs_json(res);
	printf("%s\n", res_js.c_str());
	user_msgs * res2 = get_user_msg(res_js);
	string res_js2 = format_msgs_json(res2);
	if (res_js == res_js2)
	{
		printf("true\n");
	}else{
		printf("%s\n", res_js2.c_str());
		printf("hehehe\n");
	}
	print_userMsgs(res);
}

void test_get_request_res(){
	//string json_str = "{\"result\":1,\"msg\":\"{\"count\":1,\"friends:\":[{\"name\":\"user1\"}]}\"}";
	string json_str = "{\"result\":0,\"msg\":0 }";
	printf("%s\n",json_str.c_str());
	request_res * res = get_request_res(json_str);
}

int main(){
	MYSQL * con = mysql_connect_db("127.0.0.1", "root", "123456", "user");
	MYSQL * con2 = mysql_connect_db("127.0.0.1", "root", "123456", "relation");
	MYSQL * con3 = mysql_connect_db("127.0.0.1", "root", "123456", "msg");
	mysql_test_create_relation_talbe(con2);
	test_fetch_usr_msg(con3);
	test_get_request_res();
	mysql_close(con);
	printf("here\n");
	return 0;
}