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
map<const char *, MYSQL*> mysql_init(const char *host, const char* user, const * password, vector<const char *> dbs);

#endif