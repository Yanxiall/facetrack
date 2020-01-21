/*
 * Author: Yanxia He (yanxia_he@126.com)
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <wiringPi.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>


#include <stdio.h>  
#include <sys/time.h>

#include "drawLandmarks.hpp"

 

#include <unistd.h>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std; 
#define USEPORT 1234

#define PWMPIN_DOWN 24 //GPIO.24 as the output of PWM 
#define PWMPIN_UP 1    //GPIO.1 as the output of PWM
#define DUTY_MAX 400 // duty = 400 / 4000 = 10%
#define DUTY_MIN 200 // duty = 200 / 4000 = 5%
#define PWM_PERIOD 4000 //period = PWM_PERIOD * (1/(19.2Mhz / CLOCK_DIVIDER)) = 20ms
#define CLOCK_DIVIDER 96
#define DELAY_TIME_MS 10 // 50ms
#define PWM_DUTY_STEP 3 // each step of the movement of servo
#define DEBUG

#define FACE_POSITION_MIDDLE_X 110.0 //  x coordinate of the middle point of the camera
#define FACE_POSITION_MIDDLE_Y 80.0 // y coordinate of the middle point of the camera
#define FACE_POSITION_THRESHOLD_X 26 //within the threshold the servo can not move
#define FACE_POSITION_THRESHOLD_Y 20
using namespace std;
using namespace cv;
using namespace cv::face;

long getCurrentTime()  
{  
   struct timeval tv;  
   gettimeofday(&tv,NULL);  
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;  
}  

int main(void)
{
	//set PWM 
	wiringPiSetup();
	pinMode (PWMPIN_DOWN, PWM_OUTPUT);
	pinMode (PWMPIN_UP, PWM_OUTPUT);
	pwmSetMode (PWM_MODE_MS);
	pwmSetRange (PWM_PERIOD);
	pwmSetClock (CLOCK_DIVIDER);
	int duty_down = (DUTY_MAX + DUTY_MIN) / 2;
	int duty_up = (DUTY_MAX + DUTY_MIN) / 2;
	int step_down = PWM_DUTY_STEP;
	int step_up= PWM_DUTY_STEP;
	float positionX = FACE_POSITION_MIDDLE_X;
	float positionY = FACE_POSITION_MIDDLE_Y;	
	long start;//the start time of detection 
	long end; // the end time of detection 
	
	// LBP Face Detector
	CascadeClassifier faceDetector("../lbpcascade_frontalface.xml");
	
	// create facemark
	Ptr<Facemark> facemark = FacemarkLBF::create();

	// load face detector model
	// source: https://github.com/kurnianggoro/GSOC2017
	facemark->loadModel("../lbfmodel.yaml");
	VideoCapture cap(0);//0:call raspberry camera
	
	if(!cap.isOpened())
	{
		cout<<"can't open this camera"<<endl;
		return -1;
    }	
	Mat frame,gray,dst;
	int rows;
	int cols;
	
	
	//create socket
	int serverSock = socket(AF_INET, SOCK_STREAM, 0); 
	if (serverSock < 0)
	{
		cout << "socket creation failed" << endl; 
		exit(-1); 
	}
	cout << "socket creation successfully" << endl; 
//bind IP and port
	struct sockaddr_in serverAddr; 
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
	struct sockaddr clientAddr; 
	int size = sizeof(struct sockaddr); 	
	int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, (socklen_t*)&size); 
	cout << "****NEW client touched****" << endl; 
		
	while(true)
	{
		cap>>frame;
		//get the size of the frame
		cols=frame.size().width;
		rows=frame.size().height;
		cout<<"cols:"<<cols;
		cout<<"rows:"<<rows;
		
		start=getCurrentTime();
		cout<<"start"<<start<<endl; 
		
		vector<Rect> faces; // rectangle mark for faces
		resize(frame,dst, Size(220, 160)); //change the size of the frame
		cvtColor(dst,gray,COLOR_BGR2GRAY);//convert to gray
        
		// face detector
		faceDetector.detectMultiScale(gray, faces);
		
		end=getCurrentTime();
		#ifdef DEBUG
		printf("detect face:%ld\n",end-start);
        #endif		
		// face marks
		vector< vector<Point2f> > landmarks;
		 
		// check whether face is detected 
		bool success = facemark->fit(dst,faces,landmarks);
		if(success)
		{	
			for(int i = 0; i < landmarks.size(); i++)
			{
				// draw face landmarks
				drawLandmarks(dst, landmarks[i]);
			}
			float sumx=0;//the sum of x coordinate of all the points of landmarks
			float sumy=0;// the sum of y coordinate of all the points of landmarks
			for(int i = 0; i < landmarks[0].size(); i++ )
			{
				sumx = sumx + landmarks[0][i].x;
				sumy = sumy + landmarks[0][i].y;
			}
			positionX = sumx / landmarks[0].size();// the average of x coordinate of all the points of landmarks
			positionY = sumy / landmarks[0].size();// the average of y coordinate of all the points of landmarks
			cout << "facex = "<<positionX<<endl;
			cout << "facey = "<<positionY<<endl;
			
			// the camera follows the detected first face so that the middle point of the face is in the middle of the camera 
			if (positionX < FACE_POSITION_MIDDLE_X - FACE_POSITION_THRESHOLD_X ){
				step_down = PWM_DUTY_STEP;
			}
			else if (positionX > FACE_POSITION_MIDDLE_X + FACE_POSITION_THRESHOLD_X){
				step_down = PWM_DUTY_STEP * (-1);
			}
			else 
			{
				step_down = 0;
			}
			
			if (positionY < FACE_POSITION_MIDDLE_Y - FACE_POSITION_THRESHOLD_Y)
			{
				step_up = PWM_DUTY_STEP * (-1);
			}
			else if (positionY > FACE_POSITION_MIDDLE_Y + FACE_POSITION_THRESHOLD_Y)
			{
				step_up = PWM_DUTY_STEP ;
			}
			else 
			{
				step_up = 0;	
			}
			duty_down += step_down;
			duty_up += step_up;
			if (duty_down > DUTY_MAX) 
				duty_down = DUTY_MAX;
			if (duty_down < DUTY_MIN)
				duty_down = DUTY_MIN;
			if (duty_up > DUTY_MAX)
				duty_up = DUTY_MAX;
			if (duty_up < DUTY_MIN)
				duty_up = DUTY_MIN;
			// write to change the duty of PWM
			pwmWrite(PWMPIN_DOWN, duty_down);
			pwmWrite(PWMPIN_UP, duty_up);
		    cout << "servo_up is moving with duty "<<duty_up<<endl;
			cout << "servo_down is moving with duty "<<duty_down<<endl;	
		}
		else{
			cout<<"no face detected"<<endl;
		}
		
		//imshow("face detection",dst);
		
		#ifdef DEBUG
		printf("the time of the end of processing:%ld\n",getCurrentTime());
		#endif
		//if(waitKey(10)>=0)break;
		//delay(DELAY_TIME_MS);
//************************************************************************************		
		
		cout << "wait for the message from the client.." << endl;
		
		char tack[4] = { 0 };
		
		int receivedNum = recv(clientSock, tack, 4, MSG_DONTWAIT); 
		cout << "receivedNum: " << receivedNum << endl;
		cout << tack << endl;
		
		if (strcmp(tack, "quit") == 0)
		{
			cout << "shutdown" << endl; 
			break;
		}
		
		// control the camera manually        
		if(strcmp(tack, "s") == 0)// press the key "s", the camera moves down
		{
			step_up = PWM_DUTY_STEP;
			duty_up += step_up;
		}
		else if(strcmp(tack, "w") == 0)// press the key "w", the camera moves up
		{
			
			step_up = PWM_DUTY_STEP * (-1);
			duty_up += step_up;
		}
		else if(strcmp(tack, "a") == 0) // press the key "a", the camera moves left
		{
			step_down = PWM_DUTY_STEP;
			duty_down += step_down;
		}
		else if(strcmp(tack, "d") == 0)// press the key "d", the camera moves right
		{
			step_down = PWM_DUTY_STEP * (-1);
			duty_down += step_down;
		}
		else{
			// do nothing
		}
		
		if (duty_down > DUTY_MAX) 
			duty_down = DUTY_MAX;
		if (duty_down < DUTY_MIN)
			duty_down = DUTY_MIN;
		if (duty_up > DUTY_MAX)
			duty_up = DUTY_MAX;
		if (duty_up < DUTY_MIN)
			duty_up = DUTY_MIN;
		// write to change the duty of PWM
		pwmWrite(PWMPIN_DOWN, duty_down);
		pwmWrite(PWMPIN_UP, duty_up);
       	
	    //close(clientSock); 
	
    }
	return 0;
	
}
