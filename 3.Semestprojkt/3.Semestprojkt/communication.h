#pragma once
#include <iostream>

#include <WinSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

class communication
{
public:
	communication();
	~communication();

	//sets up a socket 
	bool createSoc();                   // creates a socket (returns true/false)

	//sends a given message over socket
	void sendMsg(std::string message);  //sender en msg

	//returns a message recieved on the socket.
	std::string recvMsg();              // modtager en msg
	
	//Closes the socket
	void close();                       

private:
	SOCKET _sock, _newSocket;           // Socket
	const int _portNo = 30002;          // porten
};

