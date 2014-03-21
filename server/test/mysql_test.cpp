#include "mysql_helper.h"
#include "cmd.h"

int main(){
	MYSQL * con = msql_connect_db("127.0.0.1", "root", "123456", "user");
	cmd_register * reg_info = new cmd_register;
	reg_info->user_id = "test1";
	reg_info->password = "pass1";
	reg_info->QQ = "12345";
	insert_user_info(reg_info, con);
	return 0;
}