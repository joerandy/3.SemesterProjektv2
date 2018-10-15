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
	bool createSoc();                   // creates a socket (returns true/false)
	void sendMsg(std::string message);  //sender en msg
	std::string recvMsg();              // modtager en msg
	void close();                       // lukker forbindelsen
private:
	SOCKET _sock, _newSocket;           // Socket
	const int _portNo = 30002;          // porten
};

