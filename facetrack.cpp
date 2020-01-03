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

#include "drawLandmarks.hpp"

#define PWMPIN_down 24
#define PWMPIN_up 1
#define DUTY_MAX 400 // duty = 400 / 4000 = 10%
#define DUTY_MIN 200 // duty = 200 / 4000 = 5%
#define PWM_PERIOD 4000 //period = PWM_PERIOD * (1/(19.2Mhz / CLOCK_DIVIDER)) = 20ms
#define CLOCK_DIVIDER 96
#define DELAY_TIME_MS 10 // 50ms
#define PWM_DUTY_STEP 3


#define FACE_POSITION_MIDDLE_X 110.0 // 
#define FACE_POSITION_MIDDLE_Y 80.0 // 
#define FACE_POSITION_THRESHOLD_X 26
#define FACE_POSITION_THRESHOLD_Y 20
using namespace std;
using namespace cv;
using namespace cv::face;


#include <stdio.h>  
#include <sys/time.h>    
long getCurrentTime()  
{  
   struct timeval tv;  
   gettimeofday(&tv,NULL);  
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;  
}  
  

int main(void)
{
	// PWM 
	wiringPiSetup();
	pinMode (PWMPIN_down, PWM_OUTPUT);
	pinMode (PWMPIN_up, PWM_OUTPUT);
	pwmSetMode (PWM_MODE_MS);
	pwmSetRange (PWM_PERIOD);
	pwmSetClock (CLOCK_DIVIDER);
	int duty_down = (DUTY_MAX + DUTY_MIN) / 2;
	int duty_up = (DUTY_MAX + DUTY_MIN) / 2;
	int step_down = PWM_DUTY_STEP;
	int step_up= PWM_DUTY_STEP;
	float positionX = FACE_POSITION_MIDDLE_X;
	float positionY = FACE_POSITION_MIDDLE_Y;	
	long start;
	long end;
	long start2;
	long end2;
	long end3;
	// LBP Face Detector
	CascadeClassifier faceDetector("../lbpcascade_frontalface.xml");
	
	// create facemark
	Ptr<Facemark> facemark = FacemarkLBF::create();

	// load face detector model
	// source: https://github.com/kurnianggoro/GSOC2017
	facemark->loadModel("../lbfmodel.yaml");
	
	VideoCapture cap(0);//1锛歝all usb camera锛�0:call raspberry camera
	
	
	if(!cap.isOpened())
	{
		cout<<"can't open this camera"<<endl;
		return -1;
    }	
	Mat frame,gray,dst,blurimg;
	int rows;
	int cols;
	
	while(true)
	{
		cap>>frame;
		
		cols=frame.size().width;
		rows=frame.size().height;
		cout<<"cols:"<<cols;
		cout<<"rows:"<<rows;
		vector<Rect> faces; // rectangle mark for faces
		
		start=getCurrentTime();
		resize(frame,dst, Size(220, 160));
		
		//GaussianBlur(dst, blurimg, Size(3, 3), 0, 0);
		cvtColor(dst,gray,COLOR_BGR2GRAY);//convert to gray
		
		//GaussianBlur(dst, blurimg, Size(3, 3), 0, 0);
        
		// face detector
		faceDetector.detectMultiScale(gray, faces);
		//faceDetector.detectMultiScale(blurimg, faces);
		end=getCurrentTime();
		printf("detect face:%ld\n",end-start);  
		// face marks
		vector< vector<Point2f> > landmarks;
		 
		// check whether face is detected 
		bool success = facemark->fit(dst,faces,landmarks);
		end3=getCurrentTime();
		printf("fit time:%ld\n",end3-end); 
		 
		if(success)
		{
			start2=getCurrentTime();
			for(int i = 0; i < landmarks.size(); i++)
			{
				// draw face landmarks
				drawLandmarks(dst, landmarks[i]);
				// draw face marks
				//drawFacemarks(frame, landmarks[i], Scalar(0, 0, 255));
			}
			float sumx=0;
			float sumy=0;
			for(int i = 0; i < landmarks[0].size(); i++ )
			{
				sumx = sumx + landmarks[0][i].x;
				sumy = sumy + landmarks[0][i].y;
			}
			positionX = sumx / landmarks[0].size();
			positionY = sumy / landmarks[0].size();
			
			cout << "facex = "<<positionX<<endl;
			cout << "facey = "<<positionY<<endl;
			
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
			
			if (positionY < FACE_POSITION_MIDDLE_Y - FACE_POSITION_THRESHOLD_Y){
				step_up = PWM_DUTY_STEP * (-1);
			}
			else if (positionY > FACE_POSITION_MIDDLE_Y + FACE_POSITION_THRESHOLD_Y){
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
			pwmWrite(PWMPIN_down, duty_down);
			pwmWrite(PWMPIN_up, duty_up);
		     
			cout << "servo_up is moving with duty "<<duty_up<<endl;
			cout << "servo_down is moving with duty "<<duty_down<<endl;
			end2=getCurrentTime();
			printf("processing time:%ld\n", end2-start2);
			
		}
		else{
			cout<<"no face detected"<<endl;
		}
		
		imshow("face detection",dst);
		printf("c/c++ program:%ld\n",getCurrentTime()); 
		if(waitKey(10)>=0)break;
		//delay(DELAY_TIME_MS);
    }
	
	return 0;
}
