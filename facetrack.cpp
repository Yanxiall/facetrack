/*
 * Author: Yanxia He (yanxia_he@126.com)
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <wiringPi.h>

#include "drawLandmarks.hpp"

#define PWMPIN 1
#define DUTY_MAX 400 // duty = 400 / 4000 = 10%
#define DUTY_MIN 200 // duty = 200 / 4000 = 5%
#define PWM_PERIOD 4000 //period = PWM_PERIOD * (1/(19.2Mhz / CLOCK_DIVIDER)) = 20ms
#define CLOCK_DIVIDER 96
#define DELAY_TIME_MS 10 // 50ms
#define PWM_DUTY_STEP 2

#define FACE_POSITION_MIDDLE_X 280.0 // 
#define FACE_POSITION_MIDDLE_Y 208.0 // 

using namespace std;
using namespace cv;
using namespace cv::face;

int main(void)
{
	// PWM 
	wiringPiSetup();
	pinMode (PWMPIN, PWM_OUTPUT);
	pwmSetMode (PWM_MODE_MS);
	pwmSetRange (PWM_PERIOD);
	pwmSetClock (CLOCK_DIVIDER);
	int duty = (DUTY_MAX + DUTY_MIN) / 2;
	int step = PWM_DUTY_STEP;
	float positionX = FACE_POSITION_MIDDLE_X;
	float positionY = FACE_POSITION_MIDDLE_Y;	
	
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
	Mat frame,gray;
	while(true)
	{
		cap>>frame;
		vector<Rect> faces; // rectangle mark for faces
		cvtColor(frame,gray,COLOR_BGR2GRAY);//convert to gray
		 
		// face detector
		faceDetector.detectMultiScale(gray, faces);
		 
		// face marks
		vector< vector<Point2f> > landmarks;
		 
		// check whether face is detected 
		bool success = facemark->fit(frame,faces,landmarks);
		 
		if(success)
		{
			for(int i = 0; i < landmarks.size(); i++)
			{
				// draw face landmarks
				drawLandmarks(frame, landmarks[i]);
				// draw face marks
				//drawFacemarks(frame, landmarks[i], Scalar(0, 0, 255));
			}
			positionX = landmarks[0][0].x;
			positionY = landmarks[0][0].y;
			
			cout << "facex = "<<positionX<<endl;
			cout << "facey = "<<positionY<<endl;
			
			if (positionX < FACE_POSITION_MIDDLE_X){
				step = PWM_DUTY_STEP;
			}
			if (positionX > FACE_POSITION_MIDDLE_X){
				step = PWM_DUTY_STEP * (-1);
			}

			duty += step;
			if (duty > DUTY_MAX)
				duty = DUTY_MAX;
			if (duty < DUTY_MIN)
				duty = DUTY_MIN;
			pwmWrite(PWMPIN, duty);
			cout << "servo is moving with duty "<<duty<<endl;
		}
		else{
			cout<<"no face detected"<<endl;
		}
		
		imshow("face detection",frame);
		if(waitKey(10)>=0)break;
		//delay(DELAY_TIME_MS);
    }
	
	return 0;
}
