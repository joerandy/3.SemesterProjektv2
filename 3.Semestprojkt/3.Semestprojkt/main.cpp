#include <stdio.h>

#include <jdbc\mysql_driver.h>
#include <jdbc\cppconn\config.h>

#include "communication.h"
#include "database.h"
#include "image.h"


using namespace std;
using namespace cv;

void dbTesting() {
	string hn, un, pw;
	std::cout << "Enter DB hostname: ";
	std::cin >> hn;
	std::cout << "Enter DB username: ";
	std::cin >> un;
	std::cout << "Enter DB password: ";
	std::cin >> pw;
	database DB;
	DB.setCredentials(hn, un, pw);
	DB.addEntry(9099.0, 989, 99, 999999, true);

}
void socketTesting() {
	std::string testString = "test";
	communication com;
	com.createSoc();
	com.sendMsg("(1)");
	std::string rcvd = com.recvMsg();
	std::cout << rcvd;
	if (rcvd == testString) {
		com.sendMsg("Vi er i if, string = string");
	}
	com.sendMsg(rcvd);
	com.close();
}

void imageTesting() {
	image img;
	img.getImg();
	img.convertHSV();
	img.maskColour();
	img.convertGray();
	img.detectCircles();
	// img.getCoordinates();
	//test 
	img.display();
}

int main() {
	
	//dbTesting();
<<<<<<< HEAD

	imageTesting();

=======
	socketTesting();
>>>>>>> f7e8b56ebf2e2d31a92d37fc9573e711c1d2c0ef
	waitKey(30);
	getchar();
	return 0;
}

