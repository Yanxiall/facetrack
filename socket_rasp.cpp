#include "socket_rasp.hpp"

void socket_rasp::StartServer()
{
	
	//create socket
	serverSock = socket(AF_INET, SOCK_STREAM, 0); 
	if (serverSock < 0)
	{
		cout << "socket creation failed" << endl; 
		exit(-1); 
	}
	cout << "socket creation successfully" << endl; 
    //bind IP and port
	memset(&serverAddr, 0, sizeof(serverAddr)); 
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_port = htons(USEPORT); 
	//INADDR_ANY bind all IP
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    //bind socket
	if (bind(serverSock, 
		(struct sockaddr*)&serverAddr, 
		sizeof(struct sockaddr)) == -1)
	{
		cout << "Bind error, Port["<< USEPORT << "]" << endl; 
	       exit(-1); 
    }
	cout << "Bind successfully" << endl; 
	
   //listen
	if (listen(serverSock, 10) == -1)
	{
		cout << "Listen error!" << endl; 
	}
	cout << "Listening on port[" << USEPORT << "]" << endl; 

    //accept()
	int size = sizeof(struct sockaddr); 	
    clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, (socklen_t*)&size); 
	cout << "****NEW client touched****" << endl; 
}

void socket_rasp::ReceiveMessage(Step &control)
{	
	
	    cout << "wait for the message from the client.." << endl;
		
		char tack[255] = { 0 };
		
		int receivedNum = recv(clientSock, tack, 255, MSG_DONTWAIT); 
		cout << "receivedNum: " << receivedNum << endl;
		cout << tack << endl;
		if (strcmp(tack, "quit") == 0)
		{
			cout << "shutdown" << endl; 
			exit(-1);
		}
		
		// control the camera manually        
		if(strcmp(tack, "s") == 0)// press the key "s", the camera moves down
		{
			control.step_up = 1;	
			control.control_active = true;	
			
		}
		else if(strcmp(tack, "w") == 0)// press the key "w", the camera moves up
		{
			
			control.step_up = -1;
			control.control_active = true;	
			
		}
		else if(strcmp(tack, "a") == 0) // press the key "a", the camera moves left
		{
			control.step_down = 1;
			control.control_active = true;	
			
		}
		else if(strcmp(tack, "d") == 0)// press the key "d", the camera moves right
		{
			control.step_down = -1;
			control.control_active = true;	
			
		}
		else{
			// do nothing
			control.control_active = false;	
		}
		
		//close(clientSock);
}
