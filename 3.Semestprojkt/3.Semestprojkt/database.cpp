#include "database.h"



database::database()
{

}


database::~database()
{
}

void database::addEntry(double ballXpos, double ballYpos, double cupXpos, double cupYpos, bool success)
{
	
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect(dbhostname, dbuser, dbpw);
		con->setSchema("test");

		pstmt = con->prepareStatement("INSERT test_Data VALUES (CURRENT_TIMESTAMP,?,?,?,?,?)");
		pstmt->setDouble(1, ballXpos);
		pstmt->setDouble(2, ballYpos);
		pstmt->setDouble(3, cupXpos);
		pstmt->setDouble(4, cupYpos);
		pstmt->setBoolean(5, success);
		pstmt->executeUpdate();

		delete pstmt;
		delete con;

	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line "	<< __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}

void database::setCredentials(std::string hostname, std::string dbusername, std::string dbpass)
{
	
	/*
	dbhostname = hostname.c_str();
	dbuser = dbusername.c_str();
	dbpw = dbpw.c_str();
	std::cout << std::endl << dbpw.c_str();*/
}
