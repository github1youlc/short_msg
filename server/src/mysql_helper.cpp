/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	CreateDate: 2014/03/20
	LastModify: 2014/03/21
	TODO: All functions needed to handle mysql database
*/
/************************************************************************/


#ifndef SHORT_MSG_MYSQL_HELPER_CPP
#define SHORT_MSG_MYSQL_HELPER_CPP

#include "mysql_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  MAX_QUERY_LENGTH 2000
/************************************************************************
	TODO: mysql error handle class		
************************************************************************/
class FFError
{
public:
	std::string    Label;
	FFError( ) { Label = (char *)"Generic Error"; }
	FFError( char *message ) { Label = message; }
	~FFError() { }
	inline const char*   GetMessage  ( void )   { return Label.c_str(); }
};

/************************************************************************/
/* TODO: initialise the mysql, and get all mysql connections
		
*/
/************************************************************************/
map<const char *, MYSQL*> mysql_init_all(const char *host, const char* user, const char* password, vector<const char *> dbs)
{
	map<const char *, MYSQL*> res;
	vector<const char*>::iterator it;
	for (it = dbs.begin(); it!=dbs.end(); it++)
	{
		MYSQL * con = mysql_init(NULL);
		MYSQL * con_res;
		try
		{
			con_res = mysql_real_connect(con, host, user, password, *it, 0, NULL, 0);
			if (con_res == NULL)
			{
				throw FFError((char *)mysql_error(con));
			}

			printf("MySQL Connection Info: %s \n", mysql_get_host_info(con));
			printf("MySQL Client Info: %s \n", mysql_get_client_info());
			printf("MySQL Server Info: %s \n", mysql_get_server_info(con));
			res.insert(std::pair<const char *, MYSQL *>(*it, con));
		}catch(FFError e)
		{
			printf("cannot connect to %s, error: %s\n", *it, e.Label.c_str());
		}
	}
	return res;
}


/************************************************************************/
/* 
	Todo: connect to a mysql database and return the connection
*/
/************************************************************************/
MYSQL * mysql_connect_db(const char * host, const char * user, const char * password, const char* dbs)
{
	MYSQL * con = mysql_init(NULL);
	MYSQL * con_res;
	try
	{
		con_res = mysql_real_connect(con, host, user, password, dbs, 0, NULL, 0);
		if (con_res == NULL)
		{
			throw FFError((char *)mysql_error(con));
		}

		printf("MySQL Connection Info: %s \n", mysql_get_host_info(con));
		printf("MySQL Client Info: %s \n", mysql_get_client_info());
		printf("MySQL Server Info: %s \n", mysql_get_server_info(con));
		return con;
	}catch(FFError e)
	{
		printf("cannot connect to %s, error: %s\n", dbs, e.Label.c_str());
		return NULL;
	}
}

/************************************************************************/
/* 
	return:
		-1: mysql operation failed
		0 : user information has been stored in db
		1 : user information has not been stored in db, and the insert operation succeed
*/
/************************************************************************/
int insert_user_info(cmd_register * reg_info, MYSQL * users_db){
	MYSQL_RES * res;
	int mysql_status = 0;

	// check whether the infomation of the user exist in db
	char check_user_exist_query[MAX_QUERY_LENGTH];
	sprintf(check_user_exist_query, "select user_id from users where user_id='%s'", reg_info->user_id.c_str());
	try{
		mysql_status = mysql_query(users_db, check_user_exist_query);
		if (mysql_status)
		{
			throw FFError((char *)mysql_error(users_db));
		}
		res = mysql_store_result(users_db);
		int row_count = mysql_num_rows(res);
		if (row_count == 0)
		{
			if (res)
			{
				mysql_free_result(res);
				res = NULL;
			}
			char insert_user_info_query[MAX_QUERY_LENGTH];
			sprintf(insert_user_info_query, "insert into users set user_id='%s', password='%s'", reg_info->user_id.c_str(), reg_info->password.c_str());
			char tmp_opt[100];
			if (reg_info->email.size() != 0)
			{
				sprintf(tmp_opt, ",email='%s'", reg_info->email.c_str());
				strcat(insert_user_info_query, tmp_opt);
			}
			if (reg_info->phone.size() != 0)
			{
				sprintf(tmp_opt, ",phone='%s'", reg_info->phone.c_str());
				strcat(insert_user_info_query, tmp_opt);
			}
			if (reg_info->QQ.size() != 0)
			{
				sprintf(tmp_opt, ",QQ='%s'", reg_info->QQ.c_str());
				strcat(insert_user_info_query, tmp_opt);
			}
			try{
				mysql_status = mysql_query(users_db, insert_user_info_query);
				if (mysql_status)
				{
					throw FFError((char *)mysql_error(users_db));
				}
				return 1;
			}catch(FFError e){
				printf("insert user information, error: %s", e.Label.c_str());
				return -1;
			}

		}else if(row_count == 1)
		{
			printf("user information already stored in db\n");
			return 0;
		}
	}catch(FFError e){
		printf("check users info error:%s\n", e.Label.c_str());
		return -1;
	}

	// free mysql result
	if (res)
	{
		mysql_free_result(res);
		res = NULL;
	}
	return 2;
}


/************************************************************************
	Todo: create relation table for user
************************************************************************/
int create_relation_table(string user_id, MYSQL * con){
	char create_relation_table_query[MAX_QUERY_LENGTH];
	// the table name is: relation + user_id
	string user_id2 = "relation_";
	user_id2 = user_id2 + user_id;
	sprintf(create_relation_table_query, "create table IF NOT EXISTS %s (user_id varchar(30) PRIMARY KEY, nick_name varchar(30), in_blacklist bit)", user_id2.c_str());
	 try{
		 int mysql_status = mysql_query(con, create_relation_table_query);
		 if (mysql_status)
		 {
			 throw FFError((char *)(mysql_error(con)));
			 return -1;
		 }
		 return 1;
	 }catch(FFError e){
		printf("create relation table, error: %s\n", e.Label.c_str());
		return -1;
	 }
	return -1;
}

/************************************************************************
	TODO: create user's message table
************************************************************************/
int create_msg_table(string user_id, MYSQL * con){
	char create_msg_table_query[MAX_QUERY_LENGTH];
	// the table name is: msg + user_id
	string user_id2 = "msg_";
	user_id2 = user_id2 + user_id;
	sprintf(create_msg_table_query, "create table IF NOT EXISTS %s (msg_id bigint PRIMARY KEY auto_increment, peer_id varchar(30), data text, is_handled  tinyint(1), time datetime, msg_type int)", user_id2.c_str());
	try{
		int mysql_status = mysql_query(con, create_msg_table_query);
		if (mysql_status)
		{
			throw FFError((char *)(mysql_error(con)));
		}
		return 1;
	}catch(FFError e){
		printf("create message table, error: %s\n", e.Label.c_str());
		return -1;
	}
	return -1;
}



/************************************************************************/
/* 
	Todo: user login, save login state to database
	Return:
		1 : login successfully
		0 : user_id and password not match
		-1: mysql error
	Note:
		if the user does not log off explicitly, he or she will be regarded as login
*/
/************************************************************************/
int user_login(cmd_login * login_info, MYSQL * users_db)
{
	// the password or the user id cannot be empty
	if (login_info->user_id.size() == 0 || login_info->password.size() == 0)
	{
		return 0;
	}

	int mysql_status;
	MYSQL_RES * mysql_res;

	//check user infomation
	char check_user_info_query[MAX_QUERY_LENGTH];
	sprintf(check_user_info_query, "select * from users where user_id='%s' and password='%s'", login_info->user_id.c_str(), login_info->password.c_str());

	try{
		mysql_status = mysql_query(users_db, check_user_info_query);
		if (mysql_status)
		{
			throw FFError((char *)mysql_error(users_db));
		}
		mysql_res = mysql_store_result(users_db);
		int row_count = mysql_num_rows(mysql_res);
		if (row_count == 1)
		{
			// store information that the user has logged in
			char store_user_login_state_query[MAX_QUERY_LENGTH];
			sprintf(store_user_login_state_query, "update users set is_login=1 where user_id='%s'", login_info->user_id.c_str());
			try{
				mysql_status = mysql_query(users_db, store_user_login_state_query);
				if(mysql_status){
					throw FFError((char *) mysql_error(users_db));
				}
				if (mysql_res)
				{
					mysql_free_result(mysql_res);
				}
				return 1;
			}catch(FFError e){
				printf("store user login state, error: %s", e.Label.c_str());
				if (mysql_res)
				{
					mysql_free_result(mysql_res);
				}
				return -1;
			}
		}else{
			return 0;
		}
	}catch(FFError e){
		printf("check user information, error: %s", e.Label.c_str());
		return -1;
	}
	return 1;
}

/************************************************************************/
/* 
	TODO: handle user logoff
	Return: 
		-1 : msyql operation fail
		0  : invalid param
		1  : succeed
*/
/************************************************************************/
int user_logoff(cmd_logoff * logoff_info, MYSQL * users_db)
{
	if (logoff_info->user_id.size() == 0)
	{
		return 0;
	}
	int mysql_status = 0;
	MYSQL_RES * mysql_res;
	char user_login_state_query[MAX_QUERY_LENGTH];
	sprintf(user_login_state_query, "select is_login from users where user_id='%s'", logoff_info->user_id.c_str());
	
	mysql_status = mysql_query(users_db, user_login_state_query);
	if (mysql_status)
	{
		printf("mysql error, get user login state error : %s\n", (char*)(mysql_error(users_db)));
		return -1;
	}

	mysql_res = mysql_store_result(users_db);
	int row_count = mysql_num_rows(mysql_res);
	if (row_count != 1)
	{
		printf("No user login infomation exists\n ");
		return 0;
	}else
	{
		char user_logoff_query[MAX_QUERY_LENGTH];
		sprintf(user_logoff_query, "update users set is_login=0 where user_id='%s'", logoff_info->user_id.c_str());
		mysql_status = mysql_query(users_db, user_logoff_query);
		if (mysql_status)
		{
			printf("mysql error, update user login info error: %s\n", (char *)(mysql_error(users_db)));
			return -1;
		}
		return 1;
	}
	return 1;
}

/************************************************************************/
/* 
	TODO: fetch all unhandled messages from db
	Return:
		NULL: mysql error
		Other: correct information from database
*/
/************************************************************************/
user_msgs * fetch_user_msgs(string user_id, MYSQL * con_msg, int type)
{
	int mysql_status;
	MYSQL_RES * mysql_res;
	MYSQL_ROW row;
	char user_fetch_msgs_query[MAX_QUERY_LENGTH];
	string user_msg_table_name = "msg_";
	user_msg_table_name += user_id;
	if (type == USER_MSG_NORMAL)
	{
		sprintf(user_fetch_msgs_query, "select msg_id, peer_id, data, time from %s where is_handled=0 and msg_type = 1 order by peer_id", user_msg_table_name.c_str());
	}else if (type == USER_MSG_ALL)
	{
		sprintf(user_fetch_msgs_query, "select msg_id, peer_id, data, time from %s where is_handled=0 order by peer_id", user_msg_table_name.c_str());
	}else if (type == USER_MSG_ADDFRIEND)
	{
		sprintf(user_fetch_msgs_query, "select msg_id, peer_id, data, time from %s where is_handled=0 and msg_type = 2 order by peer_id", user_msg_table_name.c_str());
	}else{
		printf("try to fetch incorrect-type message\n");
		return NULL;
	}
	
	
	mysql_status = mysql_query(con_msg, user_fetch_msgs_query);
	if (mysql_status)
	{
		printf("fetch user msgs, error: %s\n", (char *)(mysql_error(con_msg)));
		return NULL;
	}

	
	mysql_res = mysql_store_result(con_msg);
	user_msgs *all_msgs = new user_msgs;
	user_msg * one_person_msgs = new user_msg;
	all_msgs->count_msg = 0;
	all_msgs->count_person = 0;

	one_person_msgs->count = 0;
	one_person_msgs->from = "";
	string cur_user = "";
	while(row = mysql_fetch_row(mysql_res)){
		// check if is another user msgs
		if (cur_user != row[1] && cur_user != "") // save previous user_msgs
		{
			all_msgs->user_msg.push_back(one_person_msgs);
			all_msgs->count_person++;

			one_person_msgs = new user_msg;
			one_person_msgs->count = 0;
			one_person_msgs->from = row[1];
			cur_user = row[1];
		}

		if (one_person_msgs->from == "")
		{
			one_person_msgs->from = row[1];
			cur_user = row[1];
		}
		
		string msg_id = row[0];
		char update_msg_handled[MAX_QUERY_LENGTH];
		sprintf(update_msg_handled, "update %s set is_handled=1 where msg_id=%s", user_msg_table_name.c_str(), msg_id.c_str());
		if(mysql_query(con_msg, update_msg_handled)){
			printf("update message, error: %s\n", (char *)(mysql_error(con_msg)));
		}else{
			msg * a_msg = new msg;
			a_msg->data = row[2];
			a_msg->time = row[3];
			one_person_msgs->count++;
			one_person_msgs->msgs.push_back(a_msg);

			all_msgs->count_msg++;
		}
	}
	if (one_person_msgs->from != "")
	{
		all_msgs->user_msg.push_back(one_person_msgs);
		all_msgs->count_person ++;
	}
	return all_msgs;
}

/************************************************************************/
/* 
	TODO: add friend request
	Return:
		-1: mysql operation error
		0 : user to add not exists
		1 : succeed
*/
/************************************************************************/
int user_add_friend(cmd_addFriend * addFriend_info, MYSQL *	msg_con, MYSQL * user_con){
	char insert_add_friend_query[MAX_QUERY_LENGTH];
	string msg_table_name = "msg_";
	msg_table_name = msg_table_name + addFriend_info->to;

	char check_user_exist[MAX_QUERY_LENGTH]; // check whether the user to add exists 
	sprintf(check_user_exist, "select user_id from users where user_id='%s'", addFriend_info->to.c_str());
	if(mysql_query(user_con, check_user_exist))
	{
		printf("check whether user exists error: %s\n", (char*) mysql_error(user_con));
		return -1;
	}
	MYSQL_RES * mysql_res = mysql_store_result(user_con);
	if (mysql_num_rows(mysql_res) < 1)
	{
		printf("user: %s, not eixst\n", addFriend_info->to.c_str());
		return 0;
	}

	sprintf(insert_add_friend_query, "insert into %s set peer_id='%s', data='%s', is_handled=0, time=now(), msg_type=%d", msg_table_name.c_str(), addFriend_info->from.c_str(), addFriend_info->info.c_str(), USER_MSG_ADDFRIEND);

	if (mysql_query(msg_con, insert_add_friend_query))
	{
		printf("insert add_friend message error: %s\n", (char *)mysql_error(msg_con));
		return -1;
	}
	return 1;
}

/************************************************************************/
/* 
	TODO: confirm add_friend
	Return: 
		-1: mysql operation error
		0 : add friend information not exist
		1 : accept succeed
		2 : refuse succeed
*/
/************************************************************************/
int user_confirm_addFriend (cmd_confirmAdd * confirmAdd_info, MYSQL * msg_con, MYSQL * relation_con){
	char check_add_friend_exist[MAX_QUERY_LENGTH];
	MYSQL_RES * mysql_res;
	string msg_table = "msg_";
	msg_table = msg_table + confirmAdd_info->me;
	
	sprintf(check_add_friend_exist, "select msg_id from %s where peer_id='%s'", msg_table.c_str(), confirmAdd_info->from.c_str());
	if (mysql_query(msg_con, check_add_friend_exist))
	{
		printf("check addFriend infomation failed, error: %s\n", (char *)mysql_error(msg_con));
		return -1;
	}
	mysql_res = mysql_store_result(msg_con);
	if (mysql_num_rows(mysql_res) == 0)
	{
		printf("Add Friend information not exist\n");
		return 0;
	}

	string from_msg_table = "msg_";
	from_msg_table = from_msg_table + confirmAdd_info->from;
	char info_query[MAX_QUERY_LENGTH];
	if (confirmAdd_info->is_accept == 0) // refuse to add friend  
	{
		sprintf(info_query, "insert into %s set peer_id='%s', data='refuse making friends', time=now(), is_handled=0, msg_type=2", from_msg_table.c_str(), confirmAdd_info->me.c_str());
		if (mysql_query(msg_con, info_query))
		{
			printf("inform the peer, error:%s\n", (char *)mysql_error(msg_con));
			return -1;
		}
		return 2;
	}

	string relation_table_me = "relation_";
	relation_table_me = relation_table_me + confirmAdd_info->me;
	string relation_table_from = "relation_";
	relation_table_from = relation_table_from + confirmAdd_info->from;

	char add_relation_query[MAX_QUERY_LENGTH];
	sprintf(add_relation_query, "insert into %s set user_id='%s'", relation_table_me.c_str(), confirmAdd_info->from.c_str());
	if (mysql_query(relation_con, add_relation_query))
	{
		printf("add relation, error: %s\n", (char *)mysql_error(relation_con));
		return -1;
	}

	sprintf(add_relation_query, "insert into %s set user_id='%s'", relation_table_from.c_str(), confirmAdd_info->me.c_str());
	if (mysql_query(relation_con, add_relation_query))
	{
		printf("add relation, error: %s\n", (char *)mysql_error(relation_con));
		return -1;
	}

	// inform the other one that his or her request is accept
	sprintf(info_query, "insert into %s set peer_id='%s', data='accept making friends', time=now(), is_handled=0, msg_type=2", from_msg_table.c_str(), confirmAdd_info->me.c_str());
	if (mysql_query(msg_con, info_query))
	{
		printf("inform the peer, error:%s\n", (char *)mysql_error(msg_con));
		return -1;
	}
	return 1;
}

/************************************************************************/
/* 
	TODO: get all friends of one person
	Return :
		NULl: mysql error
		Other : succeed
*/
/************************************************************************/
user_friends * user_get_friends(cmd_getFriends * getFriends_info, MYSQL * relation_con)
{
	char query_string[MAX_QUERY_LENGTH];
	string relation_table_name = "relation_";
	relation_table_name = relation_table_name + getFriends_info->me;
	sprintf(query_string, "select user_id from %s", relation_table_name.c_str());
	if (mysql_query(relation_con, query_string))
	{
		printf("get friends, error: %s\n", (char *)mysql_error(relation_con));
		return NULL;
	}
	MYSQL_RES * mysql_res = mysql_store_result(relation_con);
	MYSQL_ROW row;
	user_friends *res = new user_friends;
	res->count = 0;
	while(row = mysql_fetch_row(mysql_res)){
		res->count ++;
		res->friends.push_back(row[0]);
	}
	return res;
}

#endif