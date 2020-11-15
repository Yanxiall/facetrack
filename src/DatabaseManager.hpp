// Summary: DatabaseManager
// Author:  Yanxia
// Date:    2020-10-25

#ifndef DatabaseManager_hpp_
#define DatabaseManager_hpp_

#include <mysql.h>
#include <iostream>

#define HOST "localhost"
#define USER "pi"
#define DB "remote_monitoring_db"
#define MAXSTORGEPHOTO 40

using namespace std;

class DatabaseManager
{
public:
	DatabaseManager() = default;
	bool connectToDB();
	int insertPhoto(string photoUrl);

	int getch();
	string getpass(const char *prompt, bool show_asterisk);

private:
	MYSQL *connection, mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL_FIELD *fields;
	int query_state;
};
#endif
