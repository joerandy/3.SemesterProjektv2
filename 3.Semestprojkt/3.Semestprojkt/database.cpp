#include "database.h"
database::database()
{

}

database::~database()
{
}

void database::addEntry(double ballXpos, double ballYpos, double cupXpos, double cupYpos, double vel, double acc, bool succesCam, bool succesUser){
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement *pstmt;

		driver = get_driver_instance();
		con = driver->connect(dbhostname, dbuser, dbpw);
		con->setSchema("pongdb");

		pstmt = con->prepareStatement("INSERT testData VALUES (CURRENT_TIMESTAMP,?,?,?,?,?,?,?,?)");
		pstmt->setDouble(1, ballXpos);
		pstmt->setDouble(2, ballYpos);
		pstmt->setDouble(3, cupXpos);
		pstmt->setDouble(4, cupYpos);
		pstmt->setDouble(5, vel);
		pstmt->setDouble(6, acc);
		pstmt->setBoolean(7, succesCam);
		pstmt->setBoolean(8, succesUser);
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
