/*
 * Author: Yanxia He (yanxia_he@126.com)
 */
#include "commonInterface.hpp"
#include "DetectTrackFace.hpp"
#include "socket_rasp.hpp"
#include "drawLandmarks.hpp"
#include "PWM_Control.hpp"
#include "DetectTrackFace.hpp"
#include <unistd.h>

int main(void)
{
	//set PWM 
	Mat frame,gray,dst, img;
	
	DetectTrackFace d;
	socket_rasp s;
	PWM_Control pwmControl;
	Step steRemote, steFace;
	pwmControl.setPWM();
	
		
	d.createFacemark();
	
	VideoCapture cap(0);//0:call raspberry camera
	
	if(!cap.isOpened())
	{
		cout<<"can't open this camera"<<endl;
		return -1;
    }	
	
	s.StartServer();
	
	while(true)
	{   
		cap>>frame;
		
		resize(frame,dst, Size(220, 160)); //change the size of the frame
		cvtColor(dst,gray,COLOR_BGR2GRAY);//convert to gray		
        d.loadGrafic(dst, gray);
        img=d.FaceTrack(steFace);        
        s.ReceiveMessage(steRemote);		
		pwmControl.ControlServo(steRemote,steFace);
		//send images to client 
		s.sendimg(img);
	}
	return 0;
}
