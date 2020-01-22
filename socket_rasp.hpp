// Summary:  communication with computer
// Author:  Yanxia
// Date:    2020-01-02

#ifndef socket_rasp_hpp_
#define socket_rasp_hpp_

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <wiringPi.h>
#include "DetectTrackFace.hpp"
#include "commonInterface.hpp"

#include <stdio.h>
#include <stdlib.h>
using namespace std; 
#define USEPORT 1234

class socket_rasp{
	
	public:
	
	   void StartServer();
	   void ReceiveMessage(Step &control);
	
	private:
	  
	   int serverSock;
           struct sockaddr_in serverAddr; 
           struct sockaddr clientAddr; 
           int size;
           int clientSock;	
	
};
#endif //socket_rasp_hpp_
