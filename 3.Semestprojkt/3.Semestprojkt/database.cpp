#include "database.h"



database::database()
{
}


database::~database()
{
}

void database::addEntry(std::string dateTime, double ballXpos, double ballYpos, double cupXpos, double cupYpos, bool success)
{
	
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement *pstmt;
	
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "password");
		/* Connect to the MySQL test database */
		con->setSchema("test");

		/* '?' is the supported placeholder syntax */
		pstmt = con->prepareStatement("INSERT test_Data VALUES (CURRENT_TIMESTAMP,?,?,?,?,?)");
		
		//pstmt->setDateTime(1, dateTime); // USING PLACEHOLDER VALUE TO SHOW FORMAT, REPLACE WITH dateTime PARAMETER
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
