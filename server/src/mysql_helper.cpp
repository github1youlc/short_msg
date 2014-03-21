#ifndef SHORT_MSG_MYSQL_HELPER_CPP
#define SHORT_MSG_MYSQL_HELPER_CPP

#include "mysql_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
int insert_users_info(cmd_register * reg_info, MYSQL * users_db){
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
			if (res)
			{
				mysql_free_result(res);
				res = NULL;
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

#endif