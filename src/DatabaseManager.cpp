#include "DatabaseManager.hpp"

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int DatabaseManager::getch()
{
	int ch;
	struct termios t_old, t_new;

	tcgetattr(STDIN_FILENO, &t_old);
	t_new = t_old;
	t_new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &t_new);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
	return ch;
}

string DatabaseManager::getpass(const char *prompt, bool show_asterisk)
{
	const char BACKSPACE = 127;
	const char RETURN = 10;

	string password;
	unsigned char ch = 0;

	cout << prompt << endl;

	while ((ch = getch()) != RETURN)
	{
		if (ch == BACKSPACE)
		{
			if (password.length() != 0)
			{
				if (show_asterisk)
					cout << "\b \b";
				password.resize(password.length() - 1);
			}
		}
		else
		{
			password += ch;
			if (show_asterisk)
				cout << '*';
		}
	}
	cout << endl;
	return password;
}

bool DatabaseManager::connectToDB()
{
	//initialize database connection
	mysql_init(&mysql);
	string password = getpass("Please enter the password: ", true);

	connection = mysql_real_connect(&mysql, HOST, USER, password.c_str(), DB, 3306, 0, 0);

	if (connection == NULL)
	{
		cout << mysql_error(&mysql) << endl;
		cout << "password is incorrect, please retry" << endl;
		return false;
	}
	else
	{
		cout << "connect to database successfully" << endl;
	}

	return true;
}

int DatabaseManager::insertPhoto(string photoUrl)
{
	// get current database table
	string queryMessage = "SELECT img_url FROM log_photos";
	query_state = mysql_query(connection, queryMessage.c_str());

	result = mysql_store_result(connection);
	unsigned int numrows = mysql_num_rows(result);
	MYSQL_ROW row = mysql_fetch_row(result);
	mysql_free_result(result);

	cout << "total number of rows: " << numrows << endl;
	if (numrows > 0)
	{
		cout << "the top file name is: " << row[0] << endl;
		MYSQL_ROW rowIter;
		while ((rowIter = mysql_fetch_row(result)) != NULL)
		{
			if (rowIter[0] == photoUrl)
			{
				cout << "take multiple photos within 1 second, photo save abort" << endl;
				return 0;
			}
		}
	}

	if (numrows >= MAXSTORGEPHOTO)
	{
		bool removeFromDBSuccess = false;
		queryMessage = "DELETE FROM log_photos LIMIT 1";
		cout << "send delete query" << endl;
		query_state = mysql_query(connection, queryMessage.c_str());
		cout << "send delete query finish" << endl;
		if (query_state == 0)
		{
			removeFromDBSuccess = true;
			cout << "photo number exceed max allowed: " << MAXSTORGEPHOTO << ", the oldest photo is removed from database" << endl;
		}
		cout << "delete query feedback" << endl;

		// delete the photo from hard disk
		string topPhotoUrl = row[0];

		size_t pos = topPhotoUrl.find("2020-"); // position of "live" in str
		string filename = topPhotoUrl.substr(pos);

		string filePathToBeDeteleted = "../savePics/" + filename;

		if (removeFromDBSuccess)
		{
			if (remove(filePathToBeDeteleted.c_str()) != 0)
			{
				cout << "error when deleting " << filePathToBeDeteleted << endl;
			}
			else
			{
				cout << "File is deleted " << filePathToBeDeteleted << endl;
			}
		}
	}

	// insert a new photourl
	queryMessage = "insert into log_photos(img_url) values('" + photoUrl + "')";

	query_state = mysql_query(connection, queryMessage.c_str());

	if (query_state == 0)
	{
		cout << "insert " + photoUrl + " successfully into database" << endl;
		return 0;
	}

	return -1;
}