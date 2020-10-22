// Summary: detect and track face 
// Author:  Yanxia
// Date:    2020-01-21

#ifndef DetectTrackFace_hpp_
#define DetectTrackFace_hpp_

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
#include "commonInterface.hpp"

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

class DetectTrackFace{
	public:	
	  DetectTrackFace()
	  {
		positionX = FACE_POSITION_MIDDLE_X;
		positionY = FACE_POSITION_MIDDLE_Y;
	  };
	       
	  void createFacemark();
	  Mat FaceTrack(Step &ste);
	  
	  void createFullBodyDetector();
	  Mat detectBody(Step &ste, bool bodyTrackEnabled, bool &bodydetected);

	  void loadGrafic(Mat gray, Mat dst);
	
    private:
  
	  Mat graypic,dstpic;
	  Ptr<Facemark> facemark;
	  vector<Rect> faces;
	  vector<Rect> bodies;
	  CascadeClassifier faceDetector;
	  float positionX;
	  float positionY;
	
};
#endif //DetectTrackFace_hpp_
