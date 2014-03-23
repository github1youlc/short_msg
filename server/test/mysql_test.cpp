#include "mysql_helper.h"
#include "cmd.h"

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


int main(){
	MYSQL * con = mysql_connect_db("127.0.0.1", "root", "123456", "user");
	MYSQL * con2 = mysql_connect_db("127.0.0.1", "root", "123456", "relation");
	mysql_test_create_relation_talbe(con2);
	mysql_close(con);
	printf("here\n");
	return 0;
}