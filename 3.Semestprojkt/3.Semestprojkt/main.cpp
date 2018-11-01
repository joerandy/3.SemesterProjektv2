#include <stdio.h>

#include <jdbc\mysql_driver.h>
#include <jdbc\cppconn\config.h>

#include "communication.h"
#include "database.h"
#include "image.h"


using namespace std;

string lastProcessedMsg;
bool running = true;


void dbTesting() {
	database DB;
	DB.addEntry(11.0, 12, 13, 14, false);

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
	img.getCalibration("input.txt");

	img.getImg();
	img.convertHSV();
	img.maskColour();
	img.convertGray();
	img.detectCircles();
	////coordinates pos = img.getCoordinates();
	img.display();
}

int programLoop() {
	communication com;
	image img;
	com.createSoc();
	std::string recvdMsg, sendMsg;
	bool recvdSuccess;
	int cupZ = 0;
	int ballX, ballY;
		//foerst modtager vi cup z --V
		//svar med ball x,y --V
		//modtager "ball picked up" --V
		//svar med hastighed og vinkel
		//modtager "ball thrown"
		//svar med success (0) false, (1) true
		//modtager bruger input success, 0 eller 1
		//svar med (1)
		//modtager "new" || "exit"
	while (running) {
		recvdMsg = com.recvMsg();
		if (recvdMsg == "new") {
			cupZ = stoi(com.recvMsg());
			coordinates pos = img.getCoordinates();
			com.sendMsg("(" + to_string(pos.x) + "," + to_string(pos.y) + ")");
			recvdMsg = com.recvMsg();
			if (recvdMsg != "ball picked up") {
				cout << "Unexpected reply from client\n";
				return 1;
			}
			com.sendMsg("hej jeg er en placeholder"); //send vinkel og hastighed
			recvdMsg = com.recvMsg();
			if (recvdMsg != "ball thrown") {
				cout << "Unexpected reply from client\n";
				return 1;
			}
			com.sendMsg("hej jeg er en placeholder"); //send success, ball i cup detection
			recvdMsg = com.recvMsg();
			if (recvdMsg == "1") {
				recvdSuccess = true;
			}
			else if (recvdMsg == "0") {
				recvdSuccess = false;
			}
			else {
				cout << "Unexpected message recieved";
				return 1;
			}
			com.sendMsg("(1)");
			recvdMsg = com.recvMsg();
			if (recvdMsg == "exit") {
				running = false;
				cout << "Exit msg recieved, breaking loop";
			}
		}
	}
	cv::waitKey(30);
}

int main() {
	//dbTesting();
	//imageTesting();
	//socketTesting();

	getchar();
	return programLoop();
}