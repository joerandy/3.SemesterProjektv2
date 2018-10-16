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
	DB.addEntry(90.0, 99, 99, 999999, true);

}
void socketTesting() {
	communication com;
	com.createSoc();
}

void imageTesting() {
	image img;
	img.getImg();
	img.convertHSV();
	img.maskColour();
	img.convertGray();
	img.detectCircles();
	// img.getCoordinates();
	img.display();
}

int main() {
	
	//dbTesting();

	imageTesting();

	waitKey(30);
	return 0;
}

