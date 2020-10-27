#include "DatabaseManager.hpp"

#include <termios.h>
#include <unistd.h>

int DatabaseManager::getch() {
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
  const char BACKSPACE=127;
  const char RETURN=10;

  string password;
  unsigned char ch=0;

  cout <<prompt<<endl;

  while((ch=getch())!=RETURN)
    {
       if(ch==BACKSPACE)
         {
            if(password.length()!=0)
              {
                 if(show_asterisk)
                 cout <<"\b \b";
                 password.resize(password.length()-1);
              }
         }
       else
         {
             password+=ch;
             if(show_asterisk)
                 cout <<'*';
         }
    }
  cout <<endl;
  return password;
}



bool DatabaseManager::connectToDB()
{
	//initialize database connection
    mysql_init(&mysql);
	string password = getpass("Please enter the password: ", true);

	// the three zeros are: Which port to connect to, which socket to connect to 
	// and what client flags to use.  unless you're changing the defaults you only need to put 0 here
    connection = mysql_real_connect(&mysql, HOST, USER, password.c_str(), DB, 3306, 0, 0); 
	// Report error if failed to connect to database
    if (connection == NULL) {
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
	string queryMessage = "insert into log_photos(img_url) values('" + photoUrl + "')";
	//cout << "query =  " + queryMessage << endl;
	query_state = mysql_query(connection, queryMessage.c_str());

	if (query_state == 0)
	{
		cout << "insert " + photoUrl + " successfully into database" << endl;
		return 0;
	}

	return -1;
}