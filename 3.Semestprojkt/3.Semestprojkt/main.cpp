#include <stdio.h>

#include <jdbc\mysql_driver.h>
#include <jdbc\cppconn\config.h>
#include <cmath>
#include "communication.h"
#include "database.h"
#include "image.h"
#include "physicCalculation.h"



using namespace std;
bool running = true;

// START TEST METHODS
void dbTesting() {
	database DB;
	DB.addEntry(11.0, 12, 13, 14, 10,15, false, true);

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
// END TEST METHODS

//The main loop the program operates in. Handles the communication protocol between UR5 and computer to exchange data.
int programLoop() {
	std::cout << "Program Loop called" << endl;
	communication com;
	image img;
	com.createSoc();
	std::string recvdMsg, sendMsg;
	bool recvdSuccess;
	double cupZ = 0;
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
	coordinates posBall;
	coordinates posCup;
	physicCalculation physic;
	img.getCoordinates("cup"); //changes the member variable circles
	std::vector<cv::Vec3f> cups = img.getCups();
	//printing the cups cordinates x
	for (int i = 0; i < cups.size(); i++) {
		cout << cups[i][0] << endl;
		cout << cups[i][1] << endl;
	}

	bool firsttime = true;
			//for (int i = 0; i <= cups.size() && running; i++) {
			int i = 0;
			while (i <= cups.size()){
			if (firsttime) {
				recvdMsg = com.recvMsg();
				std::cout << recvdMsg << endl;
			}
			if (recvdMsg.substr(0,3) == "new") {
				std::cout << "Entered loop" << endl;
				cupZ = stof(recvdMsg.substr(3,recvdMsg.length()));
				posBall = img.getCoordinates("ball");
				std::cout << "The message is: " << "(" + to_string(posBall.x) + "," + to_string(posBall.y) + "," + to_string(posBall.diameter) + ")" << endl;
				com.sendMsg("(" + to_string(posBall.x) + "," + to_string(posBall.y) + "," + to_string(posBall.diameter) + ")");
				recvdMsg = com.recvMsg();
				std::cout << recvdMsg << endl;
				if (recvdMsg != "ball picked up") {
					std::cout << "Unexpected reply from client\n";
					return 1;
				}
				
				cout << cups[i][0] << " , " << cups[i][1] << endl;

				//kald fysik kode, giv kop koordinater (cups[i][0], cups[i][1]) som param og få vinkel, hastighed, acceleration
				std::vector<double> profile = physic.calc(cups[i][0], cups[i][1], cupZ);
				cout << to_string(profile[0]) + ", " + to_string(profile[1]) + ", " + to_string(profile[2]) + ", " + to_string(profile[3]) + ")" << endl; //send vinkel, hastighed, acceleration
				com.sendMsg("( " + to_string(profile[0]) + ", " + to_string(profile[1]) + ", " + to_string(profile[2]) + ", " + to_string(profile[3]) + " )");
				//com.sendMsg("(" + to_string(profile[0]) + ", 6.27, 35, 0.2)");
				
				recvdMsg = com.recvMsg();
				std::cout << recvdMsg << endl;
				if (recvdMsg != "ball thrown") {
					std::cout << "Unexpected reply from client\n";
					return 1;
				}
				com.sendMsg("(1)"); //send success, ball i cup detection
				recvdMsg = com.recvMsg();
				std::cout << recvdMsg << endl;
				if (recvdMsg == "1") {
					recvdSuccess = true;
				}
				else if (recvdMsg == "0") {
					recvdSuccess = false;
				}
				else {
					std::cout << "Unexpected message recieved";
					return 1;
				}
				com.sendMsg("(1)");

				database DB;
				DB.addEntry(posBall.x*1000, posBall.y*1000, posCup.x*1000, posBall.y*1000, physic.getAngleVel(), physic.getAngleAcc(), false, recvdSuccess);
				
				if (recvdSuccess) {
					i++;
				}

				//save data to DB here
				recvdMsg = com.recvMsg();
				std::cout << recvdMsg << endl;
				if (recvdMsg == "exit") {
					running = false;
					std::cout << "Exit msg recieved, breaking loop";
				}
				else {
					firsttime = false;
				}
			}
			
		}
	cv::waitKey(30);
}


int main() {
	//dbTesting();
	//socketTesting();
	//programLoop();


	//getBallCoordinatesTest();
	//getCupsCoordinatesTest();
	
	
	return programLoop();

}