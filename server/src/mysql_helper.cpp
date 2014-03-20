#ifndef SHORT_MSG_MYSQL_HELPER_CPP
#define SHORT_MSG_MYSQL_HELPER_CPP

#include "mysql_helper.h"

/************************************************************************/
/* TODO: initialise the mysql, and get all mysql connections
		
*/
/************************************************************************/
map<const char *, MYSQL*> mysql_init(const char *host, const char* user, const * password, vector<const char *> dbs)
{
	map<const char *, MYSQL*> res;
	vector<const char*>::iterator it;
	for (it = dbs.begin(); it!=dbs.end(); it++)
	{
		MYSQL * con = mysql_init(NULL);
		if (mysql_real_connect(con, host, user, password, *it, 0, NULL, 0))
		{
			res.insert(std::pair(const char *, MYSQL *)(*it, con));
		}else
		{
			printf("cannot connect to %s\n", *it);
		}
	}
	return res;
}

bool register(cmd_register * reg_info){
	// check whether the id is exist


	// check validation of email


	// after checking, handle the register:
	//		1. insert info to user.users
	//		2. create table of relation for the user in relation db
	//		3. create table of msg for the user in msg db
}
#endif