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
MYSQL * msql_connect_db(const char * host, const char * user, const char * password, const char* dbs)
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
		0 : user information has not been stored in db, and the insert operation failed
		1 : user information has been stored in db
		2 : user information has not been stored in db, and the insert operation succeed
*/
/************************************************************************/
int insert_user_info(cmd_register * reg_info, MYSQL * users_db){
	MYSQL_RES * res;
	int mysql_status = 0;

	// check whether the infomation of the user exist in db
	char check_user_exist_query[100];
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
			char insert_user_info_query[100];
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
				return 2;
			}catch(FFError e){
				printf("insert user information, error: %s", e.Label.c_str());
				return 0;
			}

		}else if(row_count == 1)
		{
			printf("user information already stored in db\n");
			return 1;
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


/************************************************************************/
/*
	Todo: create relation table for user
*/
/************************************************************************/
int create_relation_table(string user_id, MYSQL * con){

	return 1;
}

/************************************************************************/
/* 
	Todo: user login, save login state to database
	Return:
		1 : login successfuly
		0 : user_id and password not match
		-1: mysql error
	Note:
		if the user does not logoff explicitly, he or she will be regarded as login
*/
/************************************************************************/
int user_login(cmd_login * login_info, MYSQL * users_db)
{
	// the password or the user id cannot be empty
	if (login_info->user_id.size() == 0 || login_info->password.size() == 0)
	{
		return false;
	}

	int mysql_status;
	MYSQL_RES * mysql_res;

	//check user infomation
	char check_user_info_query[100];
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
			char store_user_login_state_query[100];
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



#endif