#include <stdio.h>

#include <jdbc\mysql_driver.h>
#include <jdbc\cppconn\config.h>
#include <cmath>
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
	com.sendMsg("(1)\n");
	std::string rcvd = com.recvMsg();
	std::cout << rcvd;
	if (rcvd == testString) {
		com.sendMsg("Vi er i if, string = string");
	}
	com.close();
}

bool imageTesting() {
	image img;
	if (img.getCalibration("input.txt")) {
		cout << "Calibration data loaded!" << endl;
	}
	else {
		cout << "Error in loading calibration data..." << endl;
		return false;
	}
	img.getImg();
	img.convertHSV();
	img.maskColour("cup");
	img.convertGray();
	img.detectCircles("cup");
	////coordinates pos = img.getCoordinates();
	img.display();
	return true;
}

int programLoop() {
	cout << "Program Loop called" << endl;
	communication com;
	image img;
	com.createSoc();
	std::string recvdMsg, sendMsg;
	bool recvdSuccess;
	int cupZ = 0;
	int ballX, ballY;
	bool running = true;
		//foerst modtager vi strengen "new" og sender intet svar men modtager lige herefter cup z --V
		//svar med ball x,y,d --V
		//modtager "ball picked up" --V
		//svar med vinkel, hastighed, acceleration 
		//modtager "ball thrown"
		//svar med success (0) false, (1) true
		//modtager bruger input success, 0 eller 1
		//svar med (1)
		//modtager "new" || "exit"
	img.getCoordinates("cup");
	std::vector<cv::Vec3f> cups = img.getCups();
	for (int i = cups.size(); i >= 0 && running; i--) {
		while (running) {
			recvdMsg = com.recvMsg();
			cout << recvdMsg << endl;
			if (recvdMsg.substr(0,3) == "new") {
				cupZ = stof(recvdMsg.substr(3,4));
				coordinates pos = img.getCoordinates("ball");
				com.sendMsg("(" + to_string(pos.x) + "," + to_string(pos.y) + "," + to_string(pos.diameter) + ")");
				recvdMsg = com.recvMsg();
				cout << recvdMsg << endl;
				if (recvdMsg != "ball picked up") {
					cout << "Unexpected reply from client\n";
					return 1;
				}

				//kald fysik kode, giv kop koordinater (cups[i][0], cups[i][1]) som param og få vinkel, hastighed, acceleration
				com.sendMsg("(5.54, 1.803, 1.644)"); //send vinkel, hastighed, acceleration
				recvdMsg = com.recvMsg();
				cout << recvdMsg << endl;
				if (recvdMsg != "ball thrown") {
					cout << "Unexpected reply from client\n";
					return 1;
				}
				com.sendMsg("(1)"); //send success, ball i cup detection
				recvdMsg = com.recvMsg();
				cout << recvdMsg << endl;
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

				//save data to DB here
				recvdMsg = com.recvMsg();
				cout << recvdMsg << endl;
				if (recvdMsg == "exit") {
					running = false;
					cout << "Exit msg recieved, breaking loop";
				}
			}
		}
		cups.pop_back();
	}
	cv::waitKey(30);
}

//simon bad om den her metode, ikke i brug endnu 
double calcWrist1ToTCPangle(float h, float d5, float d6) {
	return acos( ( pow(h+d6, 2) + pow(h+d6, 2) + pow(d5, 2) - pow(d5, 2) ) / ( 2 * (h+d6) * pow( pow(h+d6, 2) + pow(d5,2), 0.5 ) ) );
}

void getBallCoordinatesTest() {
	image img;
	coordinates pos;
	pos = img.getCoordinates("ball");
	cout << "Pos.x = " << pos.x << "  Pos.y = " << pos.y << "\n";
}

void getCupsCoordinatesTest() {
	image img;
	img.getCoordinates("cup");
	vector<cv::Vec3f> cups = img.getCups();

	for (int i = 0; i < cups.size(); i++) {
		cout << "Cup nr. " << i << ", x: " << cups[i][0];
		cout << " , y: " << cups[i][1];
		cout << " , r: " << cups[i][2] << endl;
	}
}

int main() {
	//dbTesting();
	//socketTesting();
	//programLoop();


	//getBallCoordinatesTest();
	//getCupsCoordinatesTest();


	getchar();
	return programLoop();
	return 0;
}