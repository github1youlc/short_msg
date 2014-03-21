/************************************************************************/
/* 
	Author: youlc
	Email: ylcaa@foxmail.com
	Todo: helper functions needed to handle mysql
	Note: using boost library 
*/
/************************************************************************/
#ifndef SHORT_MSG_MYSQL_HELPER_H
#define SHORT_MSG_MYSQL_HELPER_H

#include "cmd.h"
#include "mysql.h"

#include <map>
using std::map;

#include <vector>
using std::vector;

/************************************************************************/
/* TODO: initialise the mysql, and get all mysql connections
		
*/
/************************************************************************/
map<const char *, MYSQL*> mysql_init_all(const char *host, const char* user, const * password, vector<const char *> dbs);


/************************************************************************/
/* 
	Todo: connect to a mysql database and return the connection
*/
/************************************************************************/
MYSQL * msql_connect_db(const char * host, const char * user, const * password, const * char dbs);

/************************************************************************/
/* 
	return:
		-1: mysql operation failed
		0 : user information has not been stored in db, and the insert operation failed
		1 : user information has been stored in db
		2 : user information has not been stored in db, and the insert operation succeed
*/
/************************************************************************/
int insert_users_info(cmd_register * reg_info,MYSQL * users_db);
#endif