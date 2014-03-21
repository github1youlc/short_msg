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
map<const char *, MYSQL*> mysql_init_all(const char *host, const char* user, const char* password, vector<const char *> dbs);


/************************************************************************/
/* 
	Todo: connect to a mysql database and return the connection
*/
/************************************************************************/
MYSQL * msql_connect_db(const char * host, const char * user, const char* password, const char* dbs);

/************************************************************************/
/* 
	return:
		-1: mysql operation failed
		0 : user information has not been stored in db, and the insert operation failed
		1 : user information has been stored in db
		2 : user information has not been stored in db, and the insert operation succeed
*/
/************************************************************************/
int insert_user_info(cmd_register * reg_info,MYSQL * users_db);



/************************************************************************/
/* TODO: create user's relation table                                                                     */
/************************************************************************/
bool create_relation_table(string user_id, MYSQL * con);


/************************************************************************/
/* TODO: create user's msg table                                        */
/************************************************************************/
bool create_msg_table(string user_id, MYSQL * con);

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
int user_login(cmd_login * login_info, MYSQL * users_db);


#endif