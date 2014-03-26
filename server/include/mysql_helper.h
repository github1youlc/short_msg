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


#define  USER_MSG_ALL		0
#define  USER_MSG_NORMAL	1
#define	 USER_MSG_ADDFRIEND 2

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
MYSQL * mysql_connect_db(const char * host, const char * user, const char* password, const char* dbs);

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
int create_relation_table(string user_id, MYSQL * con);


/************************************************************************/
/* TODO: create user's msg table                                        */
/************************************************************************/
int create_msg_table(string user_id, MYSQL * con);

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

/************************************************************************/
/* 
	TODO: handle user logoff
	Return: 
		-1 : msyql operation fail
		0  : invalid param
		1  : succeed
*/
/************************************************************************/
int user_logoff(cmd_logoff * logoff_info, MYSQL * users_db);


/************************************************************************/
/* 
	TODO: fetch all unhandled messages from db
	Return:
		NULL: mysql error
		Other: correct information from database
*/
/************************************************************************/
user_msgs* fetch_user_msgs(string user_id, MYSQL * con_msg, int type);


/************************************************************************/
/* 
	TODO: add friend request
	Return:
		-1: mysql operation error
		1 : succeed
*/
/************************************************************************/
int user_add_friend(cmd_addFriend * addFriend_info, MYSQL *	msg_con, MYSQL * user_con);

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
int user_confirm_addFriend (cmd_confirmAdd * confirmAdd_info, MYSQL * msg_con, MYSQL * relation_con);


/************************************************************************/
/* 
	TODO: get all friends of one person
	Return :
		NULl: mysql error
		Other : succeed
*/
/************************************************************************/
user_friends * user_get_friends(cmd_getFriends * getFriends_info, MYSQL * relation_con);

#endif