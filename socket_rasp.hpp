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
#include "sha1.h"
#include "base64.h"

#include <stdio.h>
#include <stdlib.h>
using namespace std; 
#define USEPORT 8080
typedef int SOCKET;

class socket_rasp{
	public:
	
	   void StartServer();
	   void ReceiveMessage(Step &control);
	   void requestInfo(SOCKET sockClient, char * request);
	   void getKey(char *request, string clientkey);
	   void respondInfo(SOCKET sockClient, char * request);
	   void respondClient(SOCKET sockClient, char charb[],int length, bool finalFragment);
	   void WorkThread(SOCKET sockClient);
	   string translate(SOCKET sockClient);
	   void sendimg(const cv::Mat &img);
	
	private:
	  
	   int serverSock;
       struct sockaddr_in serverAddr; 
       struct sockaddr clientAddr; 
       int size;
       int clientSock;
	   int count;
	   base64 b64;	
	
};
#endif //socket_rasp_hpp_