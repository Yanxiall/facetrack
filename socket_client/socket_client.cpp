//============================================================================
// Name        : test3.cpp
// Author      : Yanxia He
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;
#pragma comment(lib, "ws2_32.lib") //load ws2_32.dll
#pragma warning(disable:4996)

int main()
{
//initialize
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

// create sock
	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0);

//****create sockAddr
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = PF_INET;
	sockAddr.sin_port = htons(1234);
	sockAddr.sin_addr.s_addr = inet_addr("192.168.0.101"); //the LAN address of raspberry

//create connection
	connect(sock, (SOCKADDR*)& sockAddr, sizeof(SOCKADDR));
	cout << "the client sends a link request" << endl;

//**** send the data to the server
	while (1)
	{
		string input;
				cout << "input something ('quit' to shutdown)" << endl;
				cin >> input;
				send(sock, input.c_str(), input.length(), 0);
				if (input == "quit")
				{
					cout << "shutdown"<<endl;
					break;
				}
			}

	    closesocket(sock);
		WSACleanup();
			return 0;


}
