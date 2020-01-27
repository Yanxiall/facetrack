#include "socket_rasp.hpp"

//get the handshake request from the client 
void socket_rasp::requestInfo(SOCKET sockClient, char * request){
    char recev[1024] = "";
    recv(sockClient, recev, 2048, 0);
    //cout << recev << endl;
    string s = recev;
    int i = s.find("Sec-WebSocket-Key");
    s = s.substr(i + 19, 24);
   
    getKey(request,s);
}

// the server returns a handshake response 
void socket_rasp::getKey(char *request, string clientkey){
    
    strcat(request, "HTTP/1.1 101 Switching Protocols\r\n");
    strcat(request, "Connection: upgrade\r\n");
    strcat(request, "Sec-WebSocket-Accept: ");
    string server_key = clientkey;
    server_key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    //SHA1 (Secure Hash Algorithm 1) to encrypt the information 
    SHA1  sha;
    unsigned int message_digest[5];
    cout <<"server_key:"<< server_key << endl;
    sha.Reset();
    sha << server_key.c_str();
    sha.Result(message_digest);
    for (int i = 0; i < 5; i++) {
        message_digest[i] = htonl(message_digest[i]);
    }
    server_key = b64.base64_encode(reinterpret_cast<const unsigned char*>(message_digest), 20);
    server_key += "\r\n";
    strcat(request, server_key.c_str());
    strcat(request, "Upgrade: websocket\r\n\r\n");
    cout << "shalserver_key:" << server_key << endl;
}

//server sends the response
void socket_rasp::respondInfo(SOCKET sockClient, char * request){
    send(sockClient, request, strlen(request), 0);
}
//server send data to client
void socket_rasp::respondClient(SOCKET sockClient, char charb[],int length, bool finalFragment){
    int addiLen = 2;
    if (length < 126) {
        addiLen = 2;
    }else if (length < 65536) {
        addiLen = 4;
    }else {
        addiLen = 10;
    }
    
    char buf[length+addiLen] = "";
    int first = 0x00;
    int tmp = 0;
    if (finalFragment) {
        first = first + 0x80;
        first = first + 0x1;
    }
    buf[0] = first;
    tmp = 1;
//encode data frame 
    unsigned int nuNum = (unsigned)length;
    if (length < 126) {
        buf[1] = length;
        tmp = 2;
    }else if (length < 65536) {
        buf[1] = 126;
        buf[2] = nuNum >> 8;
        buf[3] = length & 0xFF;
        tmp = 4;
    }else {
        
        buf[1] = 127;
        buf[2] = 0;
        buf[3] = 0;
        buf[4] = 0;
        buf[5] = 0;
        buf[6] = nuNum >> 24;
        buf[7] = nuNum >> 16;
        buf[8] = nuNum >> 8;
        buf[9] = nuNum & 0xFF;
        tmp = 10;
    }
    for (int i = 0; i < length;i++){
        buf[tmp+i]= charb[i];
       
    }
    char charbuf[length+addiLen] = "";
    memcpy(charbuf, buf, length + tmp);
    send(sockClient, charbuf, length+addiLen, 0);
}

void socket_rasp::WorkThread(SOCKET sockClient){
    char request[1024] = "";  //request Info 
    /*
    char clieninfo[2048]= ""; //respond Info after handshake
    int len = 0;              
    int point = 0;            
    int tmppoint = 0;         
    */
    //handshake protocol
    requestInfo(sockClient, request);
    respondInfo(sockClient, request);
}
//After the handshake the client sends date to server, the server translate the date from the client
string socket_rasp::translate(SOCKET sockClient)
{
    cout << "traslate start"<< endl;
    char clieninfo[2048]= ""; //client Info after handshake
    int len = 0;              
    int point = 0;            
    int tmppoint = 0;         
    len=recv(sockClient, clieninfo, 2048, MSG_DONTWAIT);
    cout << "数据长度："<<len <<endl;
    if (len>0){

        char b[4096] = "";
        memcpy(b, clieninfo, 2048);
        //get the first byte
        int first = b[point] & 0xFF;
        char opCode = (char)(first & 0x0F);  //get opCode
        if (opCode == 8){
	    cout<<"socket connection is closed"<<endl;
            close(sockClient);
        }
        //get the second byte 
        first = b[++point];
        //get payloadlength
        int payloadLength = first & 0x7F;
        if (payloadLength == 126) {
            char extended[2] = "";
            extended[0] = b[++point];
            extended[1] = b[++point];
            int shift = 0;
            payloadLength = 0;
            for (int i = 2- 1; i >= 0; i--) {
                payloadLength = payloadLength + ((extended[i] & 0xFF) << shift);
                shift += 8;
            }
        }else if (payloadLength == 127) {
            char extended[8] = "";
            tmppoint = ++point;     //temporary pointer
            point = --point;
            for (int i = 0; i < 8;i++){
                extended[i] = b[tmppoint + i];
                point++;
            }
            int shift = 0;
            payloadLength = 0;
            for (int i = 8 - 1; i >= 0; i--) {
                payloadLength = payloadLength + ((extended[i] & 0xFF) << shift);
                shift += 8;
            }
        }

       
        if ((payloadLength != 126) || (payloadLength != 127)){
            point = 1;              
        }
        //get the third byte
        char mask[4] = "";
        tmppoint = ++point;
       
        point = --point; 
        //get mask
        for (int i = 0; i < 4; i++){
            mask[i] = b[tmppoint + i];
            point++;
        }
        char changeb[4096] = "";
        int length = payloadLength;
        int readThisFragment = 1;

        //calculate the true data through mask
        while (payloadLength > 0){
            int maskbyte = b[++point];
            int index = (readThisFragment - 1) % 4;
            maskbyte = maskbyte ^ (mask[index] & 0xFF);
            changeb[readThisFragment-1] = (char)maskbyte;
            payloadLength--;
            readThisFragment++;
        }
        char a[4096] = "1231";
        char test[1024] = "";
        memcpy(test, a, strlen(a));

        //print the data from client
        char charb[4096] = "";
        memcpy(charb, changeb, length);
        string s = charb;
	return s;
    }
    return "";
}

void socket_rasp::StartServer()
{
	count = 0;
	
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
	cout << "****press any key to continue****" << endl; 
	cout << "clientSock = " << clientSock << endl;
	string x;
	cin >> x;
	WorkThread(clientSock);

}

void socket_rasp::ReceiveMessage(Step &control)
{	
	
		string tack = translate(clientSock);
		cout << "client：" << tack << endl;				
		if (tack == "quit")
		{
			cout << "shutdown" << endl; 
			exit(-1);
		}
		
		// control the camera manually        
		if(tack == "s")// press the key "s", the camera moves down
		{
			control.step_up = 1;	
			control.control_active = true;	
			
		}
		else if(tack == "w")// press the key "w", the camera moves up
		{
			
			control.step_up = -1;
			control.control_active = true;	
			
		}
		else if(tack == "a") // press the key "a", the camera moves left
		{
			control.step_down = 1;
			control.control_active = true;	
			
		}
		else if(tack == "d")// press the key "d", the camera moves right
		{
			control.step_down = -1;
			control.control_active = true;	
			
		}
		else{
			// do nothing
			control.control_active = false;	
		}		
}

void socket_rasp::sendimg(const cv::Mat &img)
{
    cout<<"sendimg1"<<endl;
    string imgstr=b64.Mat2Base64(img, "JPEG");
    //string imgstr = "I love you";
    //cout<<"imgstr = "<<imgstr<<endl;
    cout<<"len = "<<imgstr.length()<<endl;
    char temp[imgstr.length() + 1];
    strcpy(temp,imgstr.c_str());
    respondClient(clientSock, temp, imgstr.length(), true);
}


