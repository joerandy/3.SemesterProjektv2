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

		pstmt = con->prepareStatement("INSERT testdata VALUES (CURRENT_TIMESTAMP,?,?,?,?,?,?,?,?)");
		
		//std::time_t rawtime;
		//std::tm* timeinfo;
		//char buffer[80]

		//std::time(&rawtime);
		//timeinfo = std::localtime(&rawtime);

		//std::strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", timeinfo);
		//"YYYY-MM-DD HH-MM-SS"

		//std::string dateTime = "2018-12-5 12-12-12";
		//pstmt->setDateTime(1, dateTime); // USING PLACEHOLDER VALUE TO SHOW FORMAT, REPLACE WITH dateTime PARAMETER
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
