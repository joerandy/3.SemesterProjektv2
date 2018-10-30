#include <stdio.h>

#include <jdbc\mysql_driver.h>
#include <jdbc\cppconn\config.h>

#include "communication.h"
#include "database.h"
#include "image.h"


using namespace std;
using namespace cv;

void dbTesting() {
	database DB;
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
	com.close();
}

void imageTesting() {
	image img;
	img.getCalibration("input.xml");	// ændre til .txt
	img.getImg();
	img.convertHSV();
	img.maskColour();
	img.convertGray();
	img.detectCircles();
	////coordinates pos = img.getCoordinates();
	img.display();
}

int main() {
	
	//dbTesting();

	imageTesting();

	socketTesting();
	waitKey(30);
	getchar();
	return 0;
}