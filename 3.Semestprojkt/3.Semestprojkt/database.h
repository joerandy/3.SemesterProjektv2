#pragma once
#include <stdlib.h>
#include <iostream>
/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/cppconn/exception.h>
#include <jdbc/cppconn/resultset.h>
#include <jdbc/cppconn/statement.h>
#include <jdbc/cppconn/prepared_statement.h>

class database
{
public:
	database();
	~database();
	void addEntry(std::string dateTime, double ballXpos, double ballYpos, double cupXpos, double cupYpos, bool success);
	//void return type - maybe change to bool to return false if data is not saved
private:

};

