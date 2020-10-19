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

	unsigned char userRequest = 0; // 0: no request, 1: suspend, 2: quit
	bool startServerSuccess = false;
	bool startVideoSuccess = true;

	d.createFacemark();
	
	startServerSuccess = s.StartServer();

	while(startServerSuccess)
	{
		s.listenClient();

		VideoCapture cap(0);//0:call raspberry camera
		startVideoSuccess = true;

		if(!cap.isOpened())
		{
			cout<<"can't open this camera"<<endl;
			startVideoSuccess = false;
		}	
		
		while(startVideoSuccess)
		{   
			cap>>frame;
			
			resize(frame,dst, Size(220, 160)); //change the size of the frame
			cvtColor(dst,gray,COLOR_BGR2GRAY);//convert to gray		
			d.loadGrafic(gray,dst);
			img=d.FaceTrack(steFace);        
			userRequest = s.ReceiveMessage(steRemote);		
			pwmControl.ControlServo(steRemote,steFace);
			//send images to the client 
			s.sendimg(img);

			if (userRequest > 0) // if client request either suspend or quit
			{
				break;
			}
		}

		cap.release();
		s.closeClient();

		if (userRequest == 2)
		{
			break;
		}
	}
	return 0;
}
