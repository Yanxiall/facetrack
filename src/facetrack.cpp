/*
 * Author: Yanxia He (yanxia_he@126.com)
 */
#include "commonInterface.hpp"
#include "DetectTrackFace.hpp"
#include "socket_rasp.hpp"
#include "drawLandmarks.hpp"
#include "PWM_Control.hpp"
#include "DetectTrackFace.hpp"
#include "DatabaseManager.hpp"
#include <unistd.h>
#include <time.h>

#define BODY_DETECTED_COUNTER_THRESHOLD 10

using namespace std;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const string currentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d-%X", &tstruct);

	return buf;
}

int main(void)
{
	//set PWM
	Mat frame, gray, dst, img;

	DetectTrackFace d;
	socket_rasp s;
	PWM_Control pwmControl;
	Step steRemote, steFace;
	pwmControl.setPWM();

	unsigned char userRequest = 0; // 0: no request, 1: suspend, 2: quit, 3: human tracker
	bool startServerSuccess = false;
	bool startVideoSuccess = true;
	bool activeTrackMode = false;

	// body detection relevant
	bool candidateBodyDetected = false;
	bool alertTriggered = false;
	bool bodydetected = false;
	bool previousBodyDetected = false;
	bool savePicTriggered = false;
	unsigned char bodyDetectedCounter = 0;

	// database relevant
	DatabaseManager database;
	bool dbLoginStatus = false;
	while (!dbLoginStatus)
	{
		dbLoginStatus = database.connectToDB(); // true: login successful, false: login failed
	}

	//d.createFacemark();
	d.createFullBodyDetector();

	startServerSuccess = s.StartServer();

	while (startServerSuccess)
	{
		s.listenClient();
		activeTrackMode = false;

		VideoCapture cap(0); //0:call raspberry camera
		startVideoSuccess = true;

		if (!cap.isOpened())
		{
			cout << "can't open this camera" << endl;
			startVideoSuccess = false;
		}

		while (startVideoSuccess)
		{
			// video process
			cap >> frame;
			resize(frame, dst, Size(220, 160));	 //change the size of the frame
			cvtColor(dst, gray, COLOR_BGR2GRAY); //convert to gray
			d.loadGrafic(gray, dst);
			savePicTriggered = false;

			//img=d.FaceTrack(steFace);
			img = d.detectBody(steFace, activeTrackMode, bodydetected);

			if (bodydetected == true && previousBodyDetected == false)
			{

				bodyDetectedCounter = 0;
				candidateBodyDetected = true;
				//savePicTriggered = true;
			}
			else if (!bodydetected)
			{
				candidateBodyDetected = false;
				bodyDetectedCounter = 0;
				alertTriggered = false;
			}
			else if (bodydetected && candidateBodyDetected)
			{
				if (bodyDetectedCounter < 250)
				{
					bodyDetectedCounter++;
				}
			}
			else
			{
				// do nothing
			}

			previousBodyDetected = bodydetected;

			if ((bodyDetectedCounter > BODY_DETECTED_COUNTER_THRESHOLD) && (!alertTriggered))
			{
				s.sendmsg("danger");
				savePicTriggered = true;
				alertTriggered = true;
			}

			// process client request
			userRequest = s.ReceiveMessage(steRemote);

			if (userRequest == 3)
			{
				activeTrackMode = true;
			}

			if (userRequest == 4)
			{
				savePicTriggered = true;
			}

			// process servo control request
			pwmControl.ControlServo(steRemote, steFace);

			//send images to the client

			s.sendimg(img);
			if (savePicTriggered)
			{
				string filename = currentDateTime() + ".png";
				string storeFilePath = "http://www.yanxia.eu:8081/log/" + filename;
				database.insertPhoto(storeFilePath);

				imwrite("../savePics/" + filename, img);
			}

			if (userRequest == 1 || userRequest == 2) // if client request either suspend or quit
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
