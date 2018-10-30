#include "communication.h"

using namespace std;

communication::communication() {

}


communication::~communication() {

}

bool communication::createSoc() {
	WSADATA wsa;
	struct sockaddr_in server, client;
	int c;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(_portNo);

	//Bind
	if (bind(_sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
	}

	puts("Bind done");

	//Listen to incoming connections
	listen(_sock, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	c = sizeof(struct sockaddr_in);
	_newSocket = accept(_sock, (struct sockaddr *)&client, &c);
	if (_newSocket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
	}

	puts("Connection accepted");

	return 0;
}

void communication::sendMsg(string message) {
	// Send message
	const char *cMsg = message.c_str();
	send(_newSocket, cMsg, strlen(cMsg), 0);                 // send
}

string communication::recvMsg() {

	const int buffLen = 512;
	char recvBuf[buffLen];
	int iResult = recv(_newSocket, recvBuf, buffLen, 0);       // recv
	if (iResult == 0) {
		return "Message not recieved";
	}
	else {
		//the recvBuf array has no nullterminator and is filled with trashchars.
		//we look through the buffer to replace the trashchars or various escape chars with null terminator
		//this lets us compare the returned string without escape chars or trashchars
		char trashchar = -52, carriagereturn = 13, newline = 10;

		for (int i = 0; i < buffLen; i++) {
			if (recvBuf[i] == trashchar || recvBuf[i] == carriagereturn || recvBuf[i] == newline) {
				recvBuf[i] = '\0';
				string message(recvBuf);
				return message;
			}
		}
		string message(recvBuf);
		return message;
	}
	return "Failure to recieve message";
}

void communication::close() {
	sendMsg("\n Disconnecting...");
	getchar();

	closesocket(_sock);
	closesocket(_newSocket);
	WSACleanup();

}
